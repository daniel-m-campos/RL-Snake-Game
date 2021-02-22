#include "agent_factory.h"
#include "gtest/gtest.h"

TEST(RLFactoryTest, TestCreateQAgent) {
  std::unordered_map<int, std::vector<int>> state_action_map{
      {0, {0, 1}},
      {1, {1, 3}},
      {2, {2, 4}},
  };
  auto agent = AgentFactory<int, int>::CreateQAgent(state_action_map, 0.1, 0.95, 0.2, 0, 0);
}