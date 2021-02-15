#include "gtest/gtest.h"
#include "rl_factory.h"

TEST(RLFactoryTest, TestCreateQAgent) {
  std::unordered_map<int, std::vector<int>> state_action_map{
      {0, {0, 1}},
      {1, {1, 3}},
      {2, {2, 4}},
  };
  auto agent = RLFactory<int, int, double>::create_q_agent(
      state_action_map, 0.1, 0.95, 0.2, 0, 0);
}