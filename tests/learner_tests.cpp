#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "learner.h"
#include "mock_action_valuer.h"
#include "mock_policy.h"

using ::testing::Return;

TEST(LearnerTest, TestInstanitateMockPolicy) { MockPolicy<int, int> policy; }

TEST(QLearnerTest, TestPolicyUpdatedWithUniqueGreedyAction) {
  auto state = 0;
  auto action = 0;
  auto reward = -1.0;
  auto new_state = 1;
  auto greedy_action = 2;
  auto greedy_value = 7;
  auto value = 42.0;
  auto discount_factor = 0.95;
  auto step_size = 0.25;

  MockPolicy<int, int> policy;
  MockActionValuer<int, int> valuer;
  EXPECT_CALL(valuer, GetValue(state, action)).WillOnce(Return(value));
  EXPECT_CALL(valuer, ArgMax(new_state))
      .WillOnce(Return(std::vector{greedy_action}));
  EXPECT_CALL(valuer, GetValue(new_state, greedy_action))
      .WillOnce(Return(greedy_value));
  EXPECT_CALL(
      valuer,
      SetValue(state, action,
               value + step_size *
                           (reward + discount_factor * greedy_value - value)))
      .WillOnce(Return());

  QLearner<int, int, double> learner{discount_factor, step_size};
  learner.Reinforce(policy, valuer, state, action, reward, new_state);
}