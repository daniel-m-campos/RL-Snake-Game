#include "gtest/gtest.h"
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