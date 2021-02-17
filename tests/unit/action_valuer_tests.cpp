#include "action_valuer.h"
#include "gmock/gmock.h"

class SimpleActionValuerFixture : public ::testing::Test {
 public:
  std::unordered_map<int, std::vector<int>> state_actions{
      {0, {0, 1}},
      {1, {1, 3}},
      {2, {2, 4}},
  };
  std::vector<int> states{0, 1, 2};
  std::unordered_map<std::pair<int, int>, double> action_value_map{
      {{0, 0}, 0}, {{0, 1}, 1}, {{1, 1}, 0},
      {{1, 3}, 1}, {{2, 2}, 0}, {{2, 4}, 1},
  };
};

TEST_F(SimpleActionValuerFixture, TestInitValue) {
  auto init_value = 42.0;
  SimpleActionValuer action_valuer{state_actions, init_value};
  EXPECT_EQ(action_valuer.GetValue(1, 3), init_value);
}

TEST_F(SimpleActionValuerFixture, TestSetValue) {
  SimpleActionValuer action_valuer{state_actions};
  double new_value = -7.0;
  action_valuer.SetValue(0, 0, new_value);
  EXPECT_EQ(action_valuer.GetValue(0, 0), new_value);
}

TEST_F(SimpleActionValuerFixture, TestGetStates) {
  SimpleActionValuer action_valuer{state_actions};
  EXPECT_EQ(action_valuer.GetStates(), states);
}

TEST_F(SimpleActionValuerFixture, TestGetActions) {
  SimpleActionValuer action_valuer{state_actions};
  for (const auto& [state, actions] : state_actions) {
    EXPECT_EQ(action_valuer.GetActions(state), actions);
  }
}

TEST_F(SimpleActionValuerFixture, TestArgMax) {
  SimpleActionValuer action_valuer{state_actions};
  auto state = 2;
  auto action = 4;
  action_valuer.SetValue(state, action, 10.0);
  EXPECT_EQ(action_valuer.ArgMax(state), std::vector{action});
}

TEST_F(SimpleActionValuerFixture, TestActionValueMapConstructor) {
  SimpleActionValuer action_valuer{action_value_map};
  for (const auto& state : action_valuer.GetStates()) {
    for (const auto& action : action_valuer.GetActions(state)) {
      auto actual_value = action_value_map[{state, action}];
      EXPECT_EQ(action_valuer.GetValue(state, action), actual_value);
    }
  }
  EXPECT_FALSE(action_valuer.GetStates().empty());
  EXPECT_FALSE(action_valuer.GetActions(0).empty());
}