#include "game_simulator.h"

GameSimulator::GameSimulator(EnvironmentFactory environment_factory,
                             GameAgent& agent)
    : _environment_factory{std::move(environment_factory)}, _agent{agent} {}

void GameSimulator::Simulate(long num_episodes, long max_steps) {
  for (int n = 0; n < num_episodes; ++n) {
    auto environment = _environment_factory();
    RunEpisode<GameState, snake::Direction>(environment, _agent, max_steps);
  }
}