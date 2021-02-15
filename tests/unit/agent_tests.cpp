#include <memory>

#include "agent.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "learner.h"
#include "mock_action_valuer.h"
#include "mock_policy.h"

using ::testing::_;
using ::testing::Return;

template <typename S, typename A>
class MockLearner : public Learner<S, A> {
 public:
  MOCK_METHOD(void, Reinforce,
              ((const Policy<S, A>&), (ActionValuer<S, A>&), S, A, double, S),
              (override));
};

class AgentImplFixture : public ::testing::Test {
 public:
  std::shared_ptr<MockActionValuer<int, int>> valuer_ptr =
      std::make_shared<MockActionValuer<int, int>>();
  std::unique_ptr<MockPolicy<int, int>> policy_ptr =
      std::make_unique<MockPolicy<int, int>>();
  std::unique_ptr<MockLearner<int, int>> learner_ptr =
      std::make_unique<MockLearner<int, int>>();
};

TEST_F(AgentImplFixture, TestUpdate) {
  auto state = 0;
  auto action = 2;
  auto reward = -1.0;
  auto new_state = 1;

  EXPECT_CALL(*learner_ptr.get(),
              Reinforce(_, _, state, action, reward, new_state))
      .WillOnce(Return());
  AgentImpl<int, int> agent{valuer_ptr, std::move(policy_ptr),
                            std::move(learner_ptr), state, action};
  agent.Update(new_state, reward);
}

TEST_F(AgentImplFixture, TestGetAction) {
  auto state = 0;
  auto action = 1;
  auto new_state = 2;
  auto new_action = 3;

  EXPECT_CALL(*policy_ptr.get(), ParentheisesOp(new_state))
      .WillOnce(Return(new_action));
  AgentImpl<int, int> agent{valuer_ptr, std::move(policy_ptr),
                            std::move(learner_ptr), state, action};
  EXPECT_EQ(agent.GetAction(new_state), new_action);
}