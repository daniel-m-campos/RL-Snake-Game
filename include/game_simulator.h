#ifndef RLSNAKEGAME_GAME_SIMULATOR_H
#define RLSNAKEGAME_GAME_SIMULATOR_H

#include "game_environment.h"
#include "simulator.h"

class GameSimulator : public Simulator {
 public:
  using GameAgent = Agent<GameState, snake::Direction>;
  using EnvironmentFactory = std::function<GameEnvironment(void)>;

  GameSimulator(EnvironmentFactory environment_factory, GameAgent& agent);
  void Simulate(long num_episodes, long max_steps) override;

 private:
  EnvironmentFactory _environment_factory;
  GameAgent& _agent;
};

#endif  // RLSNAKEGAME_GAME_SIMULATOR_H
