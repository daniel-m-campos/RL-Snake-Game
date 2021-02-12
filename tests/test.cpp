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

TEST_F(SnakeFixture, SnakeIsInCenterOfGrid) {
  EXPECT_TRUE(snake.SnakeCell(grid_width / 2, grid_height / 2));
}

TEST_F(SnakeFixture, DefaultDirectionUpdate) {
  snake.Update();
  EXPECT_NEAR(snake.GetHeadX(), grid_width / 2, 0.001);
  EXPECT_NEAR(snake.GetHeadY(), grid_height / 2 - 0.1f, 0.001);
}

TEST_F(SnakeFixture, ChangeDirectionAndUpdate) {
  snake.SetDirection(Direction::kRight);
  snake.Update();
  EXPECT_NEAR(snake.GetHeadX(), grid_width / 2 + 0.1f, 0.001);
  EXPECT_NEAR(snake.GetHeadY(), grid_height / 2, 0.001);
}

TEST_F(SnakeFixture, GrowBodyWithUnitSpeedToRight) {
  snake.SetDirection(Direction::kRight);
  snake.SetSpeed(1.0);
  snake.GrowBody();
  snake.Update();
  EXPECT_NEAR(snake.GetHeadX(), grid_width / 2 + 1, 0.001);
  EXPECT_NEAR(snake.GetHeadY(), grid_height / 2, 0.001);
}

TEST_F(SnakeFixture, IsAliveOnInitialization) { EXPECT_TRUE(snake.IsAlive()); }

TEST_F(SnakeFixture, DiesIfEatsItself) {
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
  int operator()(int s) override { return -s; }
};

class PolicyFixture : public ::testing::Test {
 public:
  MockPolicy policy;
};

TEST_F(PolicyFixture, TestMockPolicy) { EXPECT_EQ(policy(1), -1); }

// TODO: Test Game
// TODO: Test controller
