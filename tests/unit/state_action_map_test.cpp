#include "state_action_map.h"

#include "gmock/gmock.h"

class StateActionHashMapFixture : public ::testing::Test {
 public:
  std::unordered_map<int, std::vector<int>> state_actions{
      {0, {0, 1}},
      {1, {1, 3}},
      {2, {2, 4}},
  };
  std::vector<int> states{0, 1, 2};
};

TEST_F(StateActionHashMapFixture, TestCanInitializeWithMap) {
  StateActionHashMap state_action_map{state_actions};
}

TEST_F(StateActionHashMapFixture, TestGetStates) {
  StateActionHashMap state_action_map{state_actions};
  EXPECT_EQ(state_action_map.GetStates(), states);
}

TEST_F(StateActionHashMapFixture, TestGetActions) {
  StateActionHashMap state_action_map{state_actions};
  for (const auto& [state, actions] : state_actions) {
    EXPECT_EQ(state_action_map.GetActions(state), actions);
  }
}