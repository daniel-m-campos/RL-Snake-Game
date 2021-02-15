#include "gtest/gtest.h"
#include "policy.h"

class MockPolicy : public Policy<int, int> {
 public:
  int operator()(int state) override { return -state; }
  double Probability(int action, int state) override {
    return action == -state;
  }
};

class PolicyFixture : public ::testing::Test {
 public:
  MockPolicy policy;
};

TEST_F(PolicyFixture, TestMockPolicyCall) { EXPECT_EQ(policy(1), -1); }

TEST_F(PolicyFixture, TestMockPolicyProbability) {
  EXPECT_DOUBLE_EQ(policy.Probability(1, 1), 0.0);
  EXPECT_DOUBLE_EQ(policy.Probability(2, -2), 1.0);
}

class EpsilonGreedyUniqueGreedyActionPerStateFixture : public ::testing::Test {
 public:
  std::unordered_map<int, std::vector<int>> state_actions{
      {0, {0, 1}},
      {1, {1, 3}},
      {2, {2, 4}},
  };
  std::unordered_map<std::pair<int, int>, double> action_value_map{
      {{0, 0}, 0}, {{0, 1}, 1}, {{1, 1}, 0},
      {{1, 3}, 1}, {{2, 2}, 0}, {{2, 4}, 1},
  };

  EpsilonGreedy<int, int> create_policy(double epsilon) {
    return EpsilonGreedy<int, int>{
        std::make_shared<SimpleActionValuer<int, int>>(action_value_map),
        epsilon};
    ;
  }
};

TEST_F(EpsilonGreedyUniqueGreedyActionPerStateFixture,
       TestZeroEpsilonReturnGreedyAction) {
  auto policy = create_policy(0.0);
  EXPECT_EQ(policy(0), 1);
  EXPECT_EQ(policy(1), 3);
  EXPECT_EQ(policy(2), 4);
}

TEST_F(EpsilonGreedyUniqueGreedyActionPerStateFixture,
       TestProbabilityWithSingleGreedyAction) {
  auto state = 0;
  auto epsilon = 0.1;
  auto N = state_actions[state].size();
  auto policy = create_policy(epsilon);
  EXPECT_EQ(policy.Probability(0, state), epsilon * 1 / N);
  EXPECT_EQ(policy.Probability(1, state), epsilon * 1 / N + (1 - epsilon) * 1);
}
