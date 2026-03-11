#include "trainer.h"

#include "action_valuer.h"
#include "action_valuer_factory.h"
#include "agent_factory.h"
#include "game.h"
#include "game_environment.h"
#include "game_simulator.h"
#include "io.h"
#include "snake.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

void train(std::size_t grid_width, std::size_t grid_height, int64_t num_episodes,
           int64_t max_steps, double epsilon, double discount_factor, double step_size)
{
    std::set<GameState> states;
    std::shared_ptr<ActionValuer<GameState, snake::Direction>> action_valuer =
        ActionValuerFactory<GameState, snake::Direction>::create_simple_action_valuer(
            states, snake::directions);
    auto agent = AgentFactory<GameState, snake::Direction>::create_q_agent(
        action_valuer, epsilon, discount_factor, step_size, GameState{{{0, 0}}},
        snake::Direction::kUp);

    auto environment_factory = [&]()
    {
        auto snake = std::make_unique<snake::GridSnake>(grid_width, grid_height);
        snake->set_speed(1);
        auto game =
            std::make_unique<Game>(grid_width, grid_height, std::move(snake), 0);
        return GameEnvironment(std::move(game));
    };

    GameSimulator simulator{environment_factory, *agent};
    simulator.simulate(num_episodes, max_steps);

    io::save(file_name(grid_width, grid_height), *action_valuer);
}

auto file_name(std::size_t grid_width, std::size_t grid_height) -> std::string
{
    return std::to_string(grid_width) + "x" + std::to_string(grid_height) +
           "_action_valuer"
           ".txt";
}
