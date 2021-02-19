#include "action_valuer.h"
#include "gmock/gmock.h"

class SimpleActionValuerFixture : public ::testing::Test {
 public:
  std::unordered_map<int, std::vector<int>> state_actions_map{
      {0, {0, 1}},
      {1, {1, 3}},
      {2, {2, 4}},
  };
  std::vector<int> states{0, 1, 2};
  std::unordered_map<std::pair<int, int>, double> action_value_map{
      {{0, 0}, 0}, {{0, 1}, 1}, {{1, 1}, 0},
      {{1, 3}, 1}, {{2, 2}, 0}, {{2, 4}, 1},
  };
  std::unique_ptr<StateActionMap<int, int>> state_actions{
      std::make_unique<StateActionHashMap<int, int>>(state_actions_map)};
};

TEST_F(SimpleActionValuerFixture, TestInitValue) {
  auto init_value = 42.0;
  SimpleActionValuer action_valuer{std::move(state_actions), init_value};
  EXPECT_EQ(action_valuer.GetValue(1, 3), init_value);
}

TEST_F(SimpleActionValuerFixture, TestSetValue) {
  SimpleActionValuer action_valuer{std::move(state_actions)};
  double new_value = -7.0;
  action_valuer.SetValue(0, 0, new_value);
  EXPECT_EQ(action_valuer.GetValue(0, 0), new_value);
}

TEST_F(SimpleActionValuerFixture, TestArgMax) {
  SimpleActionValuer action_valuer{std::move(state_actions)};
  auto state = 2;
  auto action = 4;
  action_valuer.SetValue(state, action, 10.0);
  EXPECT_EQ(action_valuer.ArgMax(state), std::vector{action});
}

TEST_F(SimpleActionValuerFixture, TestActionValueMapConstructor) {
  SimpleActionValuer action_valuer{std::move(state_actions), action_value_map};
  auto& state_action_map = action_valuer.GetStateActionMap();
  for (const auto& state : state_action_map.GetStates()) {
    for (const auto& action : state_action_map.GetActions(state)) {
      auto actual_value = action_value_map[{state, action}];
      EXPECT_EQ(action_valuer.GetValue(state, action), actual_value);
    }
  }
}