#include "game_simulator.h"
#include "game_environment.h"
#include "simulator.h"
#include "snake.h"
#include <cstdint>
#include <utility>

GameSimulator::GameSimulator(EnvironmentFactory environment_factory, GameAgent &agent)
    : _environment_factory{std::move(environment_factory)}, _agent{&agent}
{
}

void GameSimulator::simulate(int64_t num_episodes, int64_t max_steps)
{
    for (int64_t n = 0; n < num_episodes; ++n)
    {
        auto environment = _environment_factory();
        run_episode<GameState, snake::Direction>(environment, *_agent, max_steps);
    }
}
