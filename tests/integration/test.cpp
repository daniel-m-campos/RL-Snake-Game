#include "../src/action_valuer.cpp"
#include "../src/agent.cpp"
#include "../src/learner.cpp"
#include "../src/policy.cpp"
#include "../src/rl_factory.cpp"
#include "grid_world.h"
#include "gtest/gtest.h"

TEST(TestGridWorldSimulation, TestSimulationLoop) {
  auto agent = RLFactory<Position, Move>::CreateQAgent(
      CreateActionStateMap(), 0.1, 0.95, 0.5, Position{0, 0}, Move::kNorth);
  GridWorld environment;

  for (int n = 0; n < 100; ++n) {
    auto position = environment.GetState();
    auto action = agent->GetAction(position);
    environment.Update(action);
    agent->Update(environment.GetState(), environment.GetReward());
  }

  // TODO: check action_valuer
  // TODO: print action value function
}