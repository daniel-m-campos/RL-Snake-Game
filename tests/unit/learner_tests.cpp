#include "learner.h"
#include "mock_action_valuer.h"
#include "mock_policy.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::Return;

TEST(QLearnerTest, TestPolicyUpdatedWithUniqueGreedyAction)
{
    auto state           = 0;
    auto action          = 0;
    auto reward          = -1.0;
    auto new_state       = 1;
    auto greedy_action   = 2;
    auto greedy_value    = 7;
    auto value           = 42.0;
    auto discount_factor = 0.95;
    auto step_size       = 0.25;

    MockPolicy<int, int> policy;
    MockActionValuer<int, int> valuer;
    EXPECT_CALL(valuer, get_value(state, action)).WillOnce(Return(value));
    EXPECT_CALL(valuer, arg_max(new_state))
        .WillOnce(Return(std::vector{greedy_action}));
    EXPECT_CALL(valuer, get_value(new_state, greedy_action))
        .WillOnce(Return(greedy_value));
    EXPECT_CALL(valuer,
                set_value(state, action,
                          value + step_size * (reward + discount_factor * greedy_value -
                                               value)))
        .WillOnce(Return());

    QLearner<int, int> learner{discount_factor, step_size};
    learner.reinforce(policy, valuer, state, action, reward, new_state);
}
