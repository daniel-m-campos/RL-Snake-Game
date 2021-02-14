#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "learner.h"

using ::testing::Return;

template <typename S, typename A>
class MockPolicy : public Policy<S, A> {
 public:
  MOCK_METHOD(A, ParentheisesOp, (S state));
  A operator()(S state) override { return ParentheisesOp(state); }
  MOCK_METHOD(double, GetValue, (S state, A action), (override));
  MOCK_METHOD(void, SetValue, (S state, A action, double value), (override));
  MOCK_METHOD(double, Probability, (A action, S given_state), (override));
  MOCK_METHOD(size_t, StateSize, (), (const, override));
  MOCK_METHOD(size_t, ActionSize, (), (const, override));
};

TEST(LearnerTest, TestInstanitateMockPolicy) { MockPolicy<int, int> policy; }

//TEST(QLearnerTest, TestPolicyUpdatedWithUniqueGreedyAction) {
//  auto state = 0;
//  auto action = 0;
//  auto reward = -1.0;
//  auto new_state = 1;
//  auto new_action = 1;
//  auto value = 42.0;
//  auto discount_factor = 0.95;
//  auto step_size = 0.25;
//
//  MockPolicy<int, int> policy;
//  EXPECT_CALL(policy, GetValue(0, 0)).WillOnce(Return(value));
//  EXPECT_CALL(policy, GetValue(0, 0)).WillOnce(Return(value));
////  EXPECT_CALL(policy, SetValue(0, 0, )).WillOnce();
//}