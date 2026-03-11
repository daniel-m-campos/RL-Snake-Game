#include "parallel_trainer.h"

#include "action_valuer.h"
#include "agent_factory.h"
#include "game.h"
#include "game_environment.h"
#include "io.h"
#include "simulator.h"
#include "snake.h"
#include "trainer.h"
#include "training_config.h"

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <latch>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

using SimpleGameAV = SimpleActionValuer<GameState, snake::Direction>;
using GameAV       = ActionValuer<GameState, snake::Direction>;

class ThreadPool
{
  public:
    explicit ThreadPool(size_t num_threads)
    {
        _threads.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i)
        {
            _threads.emplace_back([this]() { worker_loop(); });
        }
    }

    ~ThreadPool()
    {
        {
            std::lock_guard lock(_mutex);
            _stop = true;
        }
        _cv.notify_all();
    }

    ThreadPool(ThreadPool const &)                     = delete;
    auto operator=(ThreadPool const &) -> ThreadPool & = delete;
    ThreadPool(ThreadPool &&)                          = delete;
    auto operator=(ThreadPool &&) -> ThreadPool &      = delete;

    void submit(std::function<void()> task)
    {
        {
            std::lock_guard lock(_mutex);
            _tasks.push(std::move(task));
        }
        _cv.notify_one();
    }

  private:
    void worker_loop()
    {
        while (true)
        {
            std::function<void()> task;
            {
                std::unique_lock lock(_mutex);
                _cv.wait(lock, [this] { return !_tasks.empty() || _stop; });
                if (_stop && _tasks.empty())
                {
                    return;
                }
                task = std::move(_tasks.front());
                _tasks.pop();
            }
            task();
        }
    }

    std::vector<std::jthread> _threads;
    std::queue<std::function<void()>> _tasks;
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _stop{false};
};

struct WorkerState
{
    std::shared_ptr<GameAV> action_valuer;
    std::unique_ptr<Agent<GameState, snake::Direction>> agent;
};

static auto create_worker(TrainingConfig const &config,
                          SimpleGameAV::action_value_map const *seed_map = nullptr,
                          std::set<GameState> const *seed_states         = nullptr)
    -> WorkerState
{
    std::set<GameState> empty_states;
    auto const &states = (seed_states != nullptr) ? *seed_states : empty_states;
    auto state_action_map =
        std::make_unique<SimpleStateActionMap<GameState, snake::Direction>>(
            states, snake::directions);
    std::shared_ptr<GameAV> action_valuer =
        (seed_map != nullptr)
            ? std::make_shared<SimpleGameAV>(std::move(state_action_map), *seed_map)
            : std::make_shared<SimpleGameAV>(std::move(state_action_map));
    auto agent = AgentFactory<GameState, snake::Direction>::create_q_agent(
        action_valuer, config.epsilon, config.discount_factor, config.step_size,
        GameState{{{.x = 0, .y = 0}}}, snake::Direction::kUp);

    return WorkerState{.action_valuer = std::move(action_valuer),
                       .agent         = std::move(agent)};
}

static auto merge_action_valuers(std::vector<WorkerState> const &workers,
                                 SimpleGameAV const *accumulator = nullptr)
    -> std::shared_ptr<GameAV>
{
    // Greedy merge: for each (state, action) pair, take the max Q-value
    // across all workers. Workers that never visited a state have 0.0
    // auto-inserted entries — we skip those by only considering non-zero
    // values, falling back to 0.0 only if all workers have zero.
    SimpleGameAV::action_value_map max_map;

    // Seed from accumulator if provided
    if (accumulator != nullptr)
    {
        for (auto const &[key, value] : accumulator->get_action_value_map())
        {
            max_map[key] = value;
        }
    }

    for (auto const &worker : workers)
    {
        auto const *simple_av =
            dynamic_cast<SimpleGameAV const *>(worker.action_valuer.get());
        if (simple_av == nullptr)
        {
            continue;
        }
        for (auto const &[key, value] : simple_av->get_action_value_map())
        {
            // Skip zero entries (unvisited state-action pairs)
            if (value == 0.0)
            {
                continue;
            }
            auto it = max_map.find(key);
            if (it == max_map.end() || it->second == 0.0 || value > it->second)
            {
                max_map[key] = value;
            }
        }
    }

    // Collect unique states, then move max_map directly as the merged result
    std::set<GameState> states;
    for (auto const &[key, _] : max_map)
    {
        states.insert(key.first);
    }

    auto state_action_map =
        std::make_unique<SimpleStateActionMap<GameState, snake::Direction>>(
            states, snake::directions);
    return std::make_shared<SimpleGameAV>(std::move(state_action_map),
                                          std::move(max_map));
}

static void run_worker(WorkerState &worker, TrainingConfig const &config,
                       size_t episodes_to_run,
                       std::shared_ptr<TrainingProgress> const &progress)
{
    for (size_t i = 0; i < episodes_to_run; ++i)
    {
        if (progress != nullptr && progress->cancel_requested.load())
        {
            break;
        }

        auto snake =
            std::make_unique<snake::GridSnake>(config.grid_width, config.grid_height);
        snake->set_speed(1);
        auto game        = std::make_unique<Game>(config.grid_width, config.grid_height,
                                                  std::move(snake), 0);
        auto environment = GameEnvironment(std::move(game));

        run_episode<GameState, snake::Direction>(environment, *worker.agent,
                                                 config.max_steps);

        if (progress != nullptr)
        {
            progress->episodes_completed.fetch_add(1);
        }
    }
}

static auto create_workers(TrainingConfig const &config, size_t num_threads,
                           std::shared_ptr<GameAV> const &accumulator)
    -> std::vector<WorkerState>
{
    std::vector<WorkerState> workers;
    workers.reserve(num_threads);
    auto const *acc_av = dynamic_cast<SimpleGameAV const *>(accumulator.get());

    SimpleGameAV::action_value_map const *seed_map = nullptr;
    std::set<GameState> state_set;
    if (acc_av != nullptr)
    {
        seed_map = &acc_av->get_action_value_map();
        for (auto const &[key, _] : *seed_map)
        {
            state_set.insert(key.first);
        }
    }

    for (size_t i = 0; i < num_threads; ++i)
    {
        workers.emplace_back(create_worker(config, seed_map,
                                           seed_map != nullptr ? &state_set : nullptr));
    }
    return workers;
}

static void run_chunk(ThreadPool &pool, std::vector<WorkerState> &workers,
                      TrainingConfig const &config, size_t num_threads,
                      int64_t episodes_this_chunk,
                      std::shared_ptr<TrainingProgress> const &progress)
{
    size_t const base_episodes = static_cast<size_t>(episodes_this_chunk) / num_threads;
    size_t const remainder     = static_cast<size_t>(episodes_this_chunk) % num_threads;

    std::latch done(static_cast<std::ptrdiff_t>(num_threads));
    for (size_t i = 0; i < num_threads; ++i)
    {
        auto episodes = base_episodes + (i < remainder ? 1 : 0);
        pool.submit(
            [&workers, &config, &progress, &done, i, episodes]()
            {
                run_worker(workers[i], config, episodes, progress);
                done.count_down();
            });
    }
    done.wait();
}

static auto merge_and_save(std::vector<WorkerState> &workers,
                           std::shared_ptr<GameAV> const &accumulator,
                           TrainingConfig const &config,
                           std::shared_ptr<TrainingProgress> const &progress)
    -> std::shared_ptr<GameAV>
{
    if (progress != nullptr)
    {
        progress->merging.store(true);
    }

    auto const *acc_av = dynamic_cast<SimpleGameAV const *>(accumulator.get());
    auto merged        = merge_action_valuers(workers, acc_av);
    workers.clear(); // free worker memory

    if (progress != nullptr)
    {
        auto const *merged_av = dynamic_cast<SimpleGameAV const *>(merged.get());
        if (merged_av != nullptr)
        {
            progress->qtable_size.store(
                static_cast<int64_t>(merged_av->get_action_value_map().size()));
        }
        progress->merging.store(false);
        progress->saving.store(true);
    }

    io::save(file_name(config.grid_width, config.grid_height), *merged);

    if (progress != nullptr)
    {
        progress->saving.store(false);
        progress->checkpoints_completed.fetch_add(1);
    }

    return merged;
}

void parallel_train(TrainingConfig const &config,
                    std::shared_ptr<TrainingProgress> progress)
{
    if (progress != nullptr)
    {
        progress->total_episodes.store(config.num_episodes);
    }

    size_t const num_threads = std::max(config.num_threads, 1);
    int64_t const chunk_size = (config.checkpoint_interval > 0)
                                   ? config.checkpoint_interval
                                   : config.num_episodes;

    int64_t episodes_done = 0;
    std::shared_ptr<GameAV> accumulator;
    ThreadPool pool(num_threads);

    while (episodes_done < config.num_episodes)
    {
        if (progress != nullptr && progress->cancel_requested.load())
        {
            break;
        }

        int64_t const episodes_this_chunk =
            std::min(chunk_size, config.num_episodes - episodes_done);

        auto workers = create_workers(config, num_threads, accumulator);
        run_chunk(pool, workers, config, num_threads, episodes_this_chunk, progress);

        if (progress != nullptr && progress->cancel_requested.load())
        {
            break;
        }

        episodes_done += episodes_this_chunk;
        accumulator = merge_and_save(workers, accumulator, config, progress);
    }

    if (progress != nullptr)
    {
        progress->training_complete.store(true);
    }
}
