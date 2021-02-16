#include <iomanip>
#include <iostream>

#include "../src/action_valuer.cpp"
#include "../src/agent.cpp"
#include "../src/learner.cpp"
#include "../src/policy.cpp"
#include "../src/rl_factory.cpp"
#include "grid_world.h"
#include "gtest/gtest.h"

void PrintValue(ActionValuer<Position, Move>* valuer, Position& state,
                Move& action) {
  std::cout << std::left << std::setw(4) << std::setfill(' ')
            << valuer->GetValue(state, action) << "\t";
}

void PrintAction(Position& state, Move& action) {
  std::unordered_map<Move, std::string> map{{
      {Move::kNorth, "^"},
      {Move::kSouth, "v"},
      {Move::kWest, "<"},
      {Move::kEast, ">"},
  }};

  std::cout << std::left << std::setw(4) << std::setfill(' ') << map[action]
            << "\t";
}

void Print(ActionValuer<Position, Move>* valuer, bool values) {
  std::cout << std::setprecision(4);
  for (int y = 4; y >= 0; --y) {
    for (int x = 0; x < 5; ++x) {
      Position state{x, y};
      auto action = valuer->ArgMax(state)[0];
      if (values) {
        PrintValue(valuer, state, action);
      } else {
        PrintAction(state, action);
      }
    }
    std::cout << std::endl;
  }
}

TEST(TestGridWorldSimulation, TestSimulationLoop) {
  auto state_action_map = CreateActionStateMap();
  auto action_valuer =
      std::make_shared<SimpleActionValuer<Position, Move>>(state_action_map);
  auto agent = RLFactory<Position, Move>::CreateQAgent(
      action_valuer, 0.7, 0.9, 0.5, Position{0, 0}, Move::kNorth);
  GridWorld environment;

  for (int n = 0; n < 10000; ++n) {
    auto position = environment.GetState();
    auto action = agent->GetAction(position);
    environment.Update(action);
    agent->Update(environment.GetState(), environment.GetReward());
  }

  Print(action_valuer.get(), true);
  Print(action_valuer.get(), false);

  std::unordered_map<Position, Move> optimal_policy{{
      {{0, 4}, Move::kEast},
      {{1, 0}, Move::kNorth},
      {{1, 1}, Move::kNorth},
      {{1, 2}, Move::kNorth},
      {{1, 3}, Move::kNorth},
      {{2, 4}, Move::kWest},
      {{3, 3}, Move::kWest},
      {{4, 3}, Move::kWest},
      {{4, 4}, Move::kWest},
  }};

  for (const auto& [state, expected_action] : optimal_policy) {
    auto actual_action = action_valuer->ArgMax(state)[0];
    EXPECT_EQ(expected_action, actual_action);
  }
}