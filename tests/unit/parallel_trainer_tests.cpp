#include "io.h"
#include "parallel_trainer.h"
#include "training_config.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <thread>

class AsyncTrainingSession
{
  public:
    AsyncTrainingSession(TrainingConfig config,
                         std::shared_ptr<TrainingProgress> progress)
        : _progress{std::move(progress)},
          _thread{[cfg = config, prog = _progress]() { parallel_train(cfg, prog); }}
    {
    }
    ~AsyncTrainingSession()
    {
        cancel();
    }
    AsyncTrainingSession(AsyncTrainingSession const &)                     = delete;
    auto operator=(AsyncTrainingSession const &) -> AsyncTrainingSession & = delete;
    AsyncTrainingSession(AsyncTrainingSession &&)                          = default;
    auto operator=(AsyncTrainingSession &&) -> AsyncTrainingSession &      = default;

    void cancel()
    {
        if (_progress != nullptr)
        {
            _progress->cancel_requested.store(true);
        }
        if (_thread.joinable())
        {
            _thread.join();
        }
    }
    [[nodiscard]] auto is_complete() const -> bool
    {
        if (_progress != nullptr)
        {
            return _progress->training_complete.load();
        }
        return !_thread.joinable();
    }

  private:
    std::shared_ptr<TrainingProgress> _progress;
    std::jthread _thread;
};

class ParallelTrainerTest : public ::testing::Test
{
  protected:
    std::filesystem::path _original_dir;
    std::filesystem::path _temp_dir;

    void SetUp() override
    {
        _original_dir = std::filesystem::current_path();
        _temp_dir = std::filesystem::temp_directory_path() / "parallel_trainer_test";
        std::filesystem::create_directories(_temp_dir);
        std::filesystem::current_path(_temp_dir);
    }

    void TearDown() override
    {
        std::filesystem::current_path(_original_dir);
        std::filesystem::remove_all(_temp_dir);
    }
};

TEST_F(ParallelTrainerTest, SingleThreadProducesFile)
{
    TrainingConfig config;
    config.grid_width   = 4;
    config.grid_height  = 4;
    config.num_episodes = 50;
    config.num_threads  = 1;

    auto progress = std::make_shared<TrainingProgress>();
    parallel_train(config, progress);

    EXPECT_TRUE(progress->training_complete.load());
    EXPECT_EQ(progress->episodes_completed.load(), 50);

    auto loaded = io::load("4x4_action_valuer.bin");
    EXPECT_TRUE(loaded.has_value());
}

TEST_F(ParallelTrainerTest, MultiThreadCompletes)
{
    TrainingConfig config;
    config.grid_width   = 4;
    config.grid_height  = 4;
    config.num_episodes = 100;
    config.num_threads  = 4;

    auto progress = std::make_shared<TrainingProgress>();
    parallel_train(config, progress);

    EXPECT_TRUE(progress->training_complete.load());
    EXPECT_EQ(progress->episodes_completed.load(), 100);
    EXPECT_GT(progress->qtable_size.load(), 0);

    auto loaded = io::load("4x4_action_valuer.bin");
    EXPECT_TRUE(loaded.has_value());
}

TEST_F(ParallelTrainerTest, CancellationStopsEarly)
{
    TrainingConfig config;
    config.grid_width   = 4;
    config.grid_height  = 4;
    config.num_episodes = 1'000'000;
    config.num_threads  = 2;

    auto progress = std::make_shared<TrainingProgress>();

    {
        AsyncTrainingSession session(config, progress);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        session.cancel();
    } // destructor joins

    EXPECT_LT(progress->episodes_completed.load(), 1'000'000);
}

TEST_F(ParallelTrainerTest, CheckpointProducesCorrectCount)
{
    TrainingConfig config;
    config.grid_width          = 4;
    config.grid_height         = 4;
    config.num_episodes        = 100;
    config.num_threads         = 2;
    config.checkpoint_interval = 25;

    auto progress = std::make_shared<TrainingProgress>();
    parallel_train(config, progress);

    EXPECT_TRUE(progress->training_complete.load());
    EXPECT_EQ(progress->episodes_completed.load(), 100);
    EXPECT_EQ(progress->checkpoints_completed.load(), 4); // 100/25 = 4 chunks
    EXPECT_GT(progress->qtable_size.load(), 0);

    auto loaded = io::load("4x4_action_valuer.bin");
    EXPECT_TRUE(loaded.has_value());
}

TEST_F(ParallelTrainerTest, CheckpointDisabledByDefault)
{
    TrainingConfig config;
    EXPECT_EQ(config.checkpoint_interval, 0);
}

TEST_F(ParallelTrainerTest, CheckpointIntervalLargerThanEpisodes)
{
    TrainingConfig config;
    config.grid_width          = 4;
    config.grid_height         = 4;
    config.num_episodes        = 50;
    config.num_threads         = 2;
    config.checkpoint_interval = 1000;

    auto progress = std::make_shared<TrainingProgress>();
    parallel_train(config, progress);

    EXPECT_TRUE(progress->training_complete.load());
    EXPECT_EQ(progress->episodes_completed.load(), 50);
    EXPECT_EQ(progress->checkpoints_completed.load(), 1); // single chunk
}
