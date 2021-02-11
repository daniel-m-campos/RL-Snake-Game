#include <snake.h>

#include <iostream>
#include "gtest/gtest.h"

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

TEST_F(SnakeFixture, PositionAfterConstructor) {
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
  snake.ChangeSpeed(0.9);
  snake.GrowBody();
  snake.Update();
  EXPECT_NEAR(snake.GetHeadX(), grid_width / 2 + 1, 0.001);
  EXPECT_NEAR(snake.GetHeadY(), grid_height / 2, 0.001);
}
}  // namespace SnakeTests

// TODO: Test Game
// TODO: Test controller
