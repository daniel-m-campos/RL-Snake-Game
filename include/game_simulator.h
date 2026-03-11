#pragma once

#include "game_environment.h"
#include "simulator.h"
#include <cstdint>
#include <functional>

class GameSimulator : public Simulator
{
  public:
    using GameAgent          = Agent<GameState, snake::Direction>;
    using EnvironmentFactory = std::function<GameEnvironment(void)>;

    GameSimulator(EnvironmentFactory environment_factory, GameAgent &agent);
    // NOLINT(bugprone-easily-swappable-parameters)
    void simulate(int64_t num_episodes, int64_t max_steps) override;

  private:
    EnvironmentFactory _environment_factory;
    GameAgent *_agent;
};
