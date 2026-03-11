#include "action_valuer.h"
#include "gmock/gmock.h"

class SimpleActionValuerFixture : public ::testing::Test
{
  public:
    std::unordered_map<int, std::vector<int>> state_actions_map{
        {0, {0, 1}},
        {1, {1, 3}},
        {2, {2, 4}},
    };
    std::vector<int> states{0, 1, 2};
    std::unordered_map<std::pair<int, int>, double> action_value_map{
        {{0, 0}, 0}, {{0, 1}, 1}, {{1, 1}, 0}, {{1, 3}, 1}, {{2, 2}, 0}, {{2, 4}, 1},
    };
    std::unique_ptr<StateActionMap<int, int>> state_actions{
        std::make_unique<StateActionHashMap<int, int>>(state_actions_map)};
};

TEST_F(SimpleActionValuerFixture, TestInitValue)
{
    auto init_value = 42.0;
    SimpleActionValuer action_valuer{std::move(state_actions), init_value};
    EXPECT_EQ(action_valuer.get_value(1, 3), init_value);
}

TEST_F(SimpleActionValuerFixture, TestSetValue)
{
    SimpleActionValuer action_valuer{std::move(state_actions)};
    double new_value = -7.0;
    action_valuer.set_value(0, 0, new_value);
    EXPECT_EQ(action_valuer.get_value(0, 0), new_value);
}

TEST_F(SimpleActionValuerFixture, TestArgMax)
{
    SimpleActionValuer action_valuer{std::move(state_actions)};
    auto state  = 2;
    auto action = 4;
    action_valuer.set_value(state, action, 10.0);
    EXPECT_EQ(action_valuer.arg_max(state), std::vector{action});
}

TEST_F(SimpleActionValuerFixture, TestActionValueMapConstructor)
{
    SimpleActionValuer action_valuer{std::move(state_actions), action_value_map};
    auto &state_action_map = action_valuer.get_state_action_map();
    for (auto const &state : state_action_map.get_states())
    {
        for (auto const &action : state_action_map.get_actions(state))
        {
            auto actual_value = action_value_map[{state, action}];
            EXPECT_EQ(action_valuer.get_value(state, action), actual_value);
        }
    }
}
