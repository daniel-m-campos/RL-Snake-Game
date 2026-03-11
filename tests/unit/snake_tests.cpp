#include "snake.h"
#include "gtest/gtest.h"

namespace SnakeTests
{

using namespace snake;

class SnakeFixture : public ::testing::Test
{
  public:
    int grid_width;
    int grid_height;
    GridSnake snake;
    SnakeFixture(int grid_width = 10, int grid_height = 15)
        : grid_width(grid_width), grid_height(grid_height),
          snake(grid_width, grid_height)
    {
    }
};

TEST_F(SnakeFixture, TestSnakeIsInCenterOfGrid)
{
    EXPECT_TRUE(snake.snake_cell(grid_width / 2, grid_height / 2));
}

TEST_F(SnakeFixture, TestDefaultDirectionUpdate)
{
    snake.update();
    EXPECT_NEAR(snake.get_head_x(), grid_width / 2, 0.001);
    EXPECT_NEAR(snake.get_head_y(), grid_height / 2 - 0.1f, 0.001);
}

TEST_F(SnakeFixture, TestChangeDirectionAndUpdate)
{
    snake.set_direction(Direction::kRight);
    snake.update();
    EXPECT_NEAR(snake.get_head_x(), grid_width / 2 + 0.1f, 0.001);
    EXPECT_NEAR(snake.get_head_y(), grid_height / 2, 0.001);
}

TEST_F(SnakeFixture, TestGrowBodyWithUnitSpeedToRight)
{
    snake.set_direction(Direction::kRight);
    snake.set_speed(1.0);
    snake.grow_body();
    snake.update();
    EXPECT_NEAR(snake.get_head_x(), grid_width / 2 + 1, 0.001);
    EXPECT_NEAR(snake.get_head_y(), grid_height / 2, 0.001);
}

TEST_F(SnakeFixture, TestIsAliveOnInitialization)
{
    EXPECT_TRUE(snake.is_alive());
}

TEST_F(SnakeFixture, TestDiesIfEatsItself)
{
    snake.set_speed(1.0);
    for (int i = 0; i < 4; ++i)
    {
        snake.grow_body();
        snake.update();
    }
    EXPECT_EQ(snake.size(), 5);
    for (auto direction : {Direction::kRight, Direction::kDown, Direction::kLeft})
    {
        snake.set_direction(direction);
        snake.update();
    }
    EXPECT_FALSE(snake.is_alive());
}

} // namespace SnakeTests
