#include "gridworld.h"

#include <iomanip>
#include <iostream>

#include "../src/action_valuer.cpp"
#include "../src/agent.cpp"
#include "../src/learner.cpp"
#include "../src/policy.cpp"
#include "../src/rl_factory.cpp"
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
  std::cout << std::setprecision(3);
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

  // Data from Figure 3.5 page 65 of Sutton and Bart0 2018
  std::unordered_map<Position, double> value_function{{
      {{0, 0}, 14.4}, {{0, 1}, 16.0}, {{0, 2}, 17.8}, {{0, 3}, 19.8},
      {{0, 4}, 22.0}, {{1, 0}, 16.0}, {{1, 1}, 17.8}, {{1, 2}, 19.8},
      {{1, 3}, 22.0}, {{1, 4}, 24.4}, {{2, 0}, 14.4}, {{2, 1}, 16.0},
      {{2, 2}, 17.8}, {{2, 3}, 19.8}, {{2, 4}, 22.0}, {{3, 0}, 13.0},
      {{3, 1}, 14.4}, {{3, 2}, 16.0}, {{3, 3}, 17.8}, {{3, 4}, 19.4},
      {{4, 0}, 11.7}, {{4, 1}, 13.0}, {{4, 2}, 14.4}, {{4, 3}, 16.0},
      {{4, 4}, 17.5},
  }};

  for (const auto& [state, expected_action] : optimal_policy) {
    auto actual_action = action_valuer->ArgMax(state)[0];
    EXPECT_EQ(expected_action, actual_action);
    auto actual_value = action_valuer->GetValue(state, actual_action);
    EXPECT_NEAR(value_function[state], actual_value, 0.1);
  }
}