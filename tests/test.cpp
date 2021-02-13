#include <iostream>

#include "gtest/gtest.h"
#include "policy.h"
#include "snake.h"

namespace SnakeTests {

using namespace snake;

class SnakeFixture : public ::testing::Test {
 public:
  int grid_width;
  int grid_height;
  GridSnake snake;
  SnakeFixture(int grid_width = 10, int grid_height = 15)
      : grid_width(grid_width),
        grid_height(grid_height),
        snake(grid_width, grid_height) {}
};

TEST_F(SnakeFixture, TestSnakeIsInCenterOfGrid) {
  EXPECT_TRUE(snake.SnakeCell(grid_width / 2, grid_height / 2));
}

TEST_F(SnakeFixture, TestDefaultDirectionUpdate) {
  snake.Update();
  EXPECT_NEAR(snake.GetHeadX(), grid_width / 2, 0.001);
  EXPECT_NEAR(snake.GetHeadY(), grid_height / 2 - 0.1f, 0.001);
}

TEST_F(SnakeFixture, TestChangeDirectionAndUpdate) {
  snake.SetDirection(Direction::kRight);
  snake.Update();
  EXPECT_NEAR(snake.GetHeadX(), grid_width / 2 + 0.1f, 0.001);
  EXPECT_NEAR(snake.GetHeadY(), grid_height / 2, 0.001);
}

TEST_F(SnakeFixture, TestGrowBodyWithUnitSpeedToRight) {
  snake.SetDirection(Direction::kRight);
  snake.SetSpeed(1.0);
  snake.GrowBody();
  snake.Update();
  EXPECT_NEAR(snake.GetHeadX(), grid_width / 2 + 1, 0.001);
  EXPECT_NEAR(snake.GetHeadY(), grid_height / 2, 0.001);
}

TEST_F(SnakeFixture, TestIsAliveOnInitialization) {
  EXPECT_TRUE(snake.IsAlive());
}

TEST_F(SnakeFixture, TestDiesIfEatsItself) {
  snake.SetSpeed(1.0);
  for (int i = 0; i < 4; ++i) {
    snake.GrowBody();
    snake.Update();
  }
  EXPECT_EQ(snake.Size(), 5);
  for (auto direction :
       {Direction::kRight, Direction::kDown, Direction::kLeft}) {
    snake.SetDirection(direction);
    snake.Update();
  }
  EXPECT_FALSE(snake.IsAlive());
}

}  // namespace SnakeTests

class MockPolicy : public Policy<int, int> {
 public:
  int operator()(int state) override { return -state; }
  double Probability(int action, int state) override {
    return action == -state;
  }
  double GetValue(int state, int action) override { return 0; }
  void SetValue(int state, int action, double value) override {}
  size_t StateSize() const override { return std::numeric_limits<int>::max(); }
  size_t ActionSize() const override { return std::numeric_limits<int>::max(); }
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

class EpsilonGreedyZeroValueFunctionFixture : public ::testing::Test {
 public:
  double epsilon = 0.1;
  EpsilonGreedy<int, int> policy{3, 4, epsilon};
};

TEST_F(EpsilonGreedyZeroValueFunctionFixture, TestInitializedValue) {
  for (int action = 0; action < policy.ActionSize(); ++action) {
    for (int state = 0; state < policy.StateSize(); ++state) {
      EXPECT_DOUBLE_EQ(policy.GetValue(action, state), 0.0);
    }
  }
}

TEST_F(EpsilonGreedyZeroValueFunctionFixture, TestInitializedProbability) {
  for (int action = 0; action < policy.ActionSize(); ++action) {
    for (int state = 0; state < policy.StateSize(); ++state) {
      EXPECT_NEAR(policy.Probability(action, state), 1.0 / policy.ActionSize(),
                  1e-6);
    }
  }
}

TEST_F(EpsilonGreedyZeroValueFunctionFixture,
       TestOperatorCallLessThanActionSize) {
  EXPECT_LT(policy(2), policy.ActionSize());
}

class EpsilonGreedyUniqueGreedyActionPerStateFixture : public ::testing::Test {
 public:
  std::unordered_map<int, std::vector<int>> state_actions{
      {0, {0, 1}},
      {1, {1, 3}},
      {2, {2, 4}},
  };
  std::unordered_map<std::pair<int, int>, double> action_values{
      {{0, 0}, 0}, {{0, 1}, 1}, {{1, 1}, 0},
      {{1, 3}, 1}, {{2, 2}, 0}, {{2, 4}, 1},
  };
};

TEST_F(EpsilonGreedyUniqueGreedyActionPerStateFixture,
       TestZeroEpsilonReturnGreedyAction) {
  EpsilonGreedy<int, int> policy{state_actions, action_values, 0.0};
  EXPECT_EQ(policy(0), 1);
  EXPECT_EQ(policy(1), 3);
  EXPECT_EQ(policy(2), 4);
}

TEST_F(EpsilonGreedyUniqueGreedyActionPerStateFixture,
       TestProbabilityWithSingleGreedyAction) {
  auto state = 0;
  auto epsilon = 0.1;
  auto N = state_actions[state].size();
  EpsilonGreedy<int, int> policy{state_actions, action_values, epsilon};
  EXPECT_EQ(policy.Probability(0, state), epsilon * 1 / N);
  EXPECT_EQ(policy.Probability(1, state), epsilon * 1 / N + (1 - epsilon) * 1);
}

TEST_F(EpsilonGreedyUniqueGreedyActionPerStateFixture,
       TestValueWithSingleGreedyAction) {
  EpsilonGreedy<int, int> policy{state_actions, action_values, 0.1};
  for (const auto& [key, value] : action_values) {
    auto [state, action] = key;
    EXPECT_EQ(policy.GetValue(state, action), value);
  }
}

TEST_F(EpsilonGreedyUniqueGreedyActionPerStateFixture, TestSetValues) {
  EpsilonGreedy<int, int> policy{state_actions, action_values, 0.1};
  double new_value = 42.0;
  for (const auto& [key, value] : action_values) {
    auto [state, action] = key;
    policy.SetValue(state, action, new_value);
    EXPECT_EQ(policy.GetValue(state, action), new_value);
    ++new_value;
  }
}

// TODO: Test Game
// TODO: Test controller
