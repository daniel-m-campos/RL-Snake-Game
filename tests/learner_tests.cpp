#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "learner.h"

using ::testing::Return;

template <typename S, typename A>
class MockPolicy : public Policy<S, A> {
 public:
  MOCK_METHOD(A, ParentheisesOp, (S state));
  A operator()(S state) override { return ParentheisesOp(state); }
  MOCK_METHOD(double, Probability, (A action, S given_state), (override));
};

template <typename S, typename A>
class MockActionValuer : public ActionValuer<S, A> {
 public:
  MOCK_METHOD(double, GetValue, (S state, A action), (override));
  MOCK_METHOD(void, SetValue, (S state, A action, double value), (override));
  MOCK_METHOD(std::vector<A>, ArgMax, (S state), (override));
  MOCK_METHOD(std::vector<S>, GetStates, (), (override));
  MOCK_METHOD(std::vector<A>, GetActions, (S state), (override));
};

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