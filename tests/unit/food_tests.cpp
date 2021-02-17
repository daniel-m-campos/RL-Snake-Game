#include "food.h"
#include "gtest/gtest.h"

using namespace snake;

TEST(FoodTest, TestTryFeedSnake) {
  auto grid_width = 10;
  auto grid_height = 10;
  Point location{grid_width / 2, grid_width / 2};

  Food food{static_cast<size_t>(grid_width), static_cast<size_t>(grid_height),
            location};
  GridSnake snake{grid_width, grid_height};
  EXPECT_TRUE(food.TryFeed(&snake));
  EXPECT_FALSE(food.TryFeed(&snake));
}

TEST(FoodTest, TestGetLocation) {
  const Point& point = Point{2, 3};
  Food food{10, 10, point};
  EXPECT_EQ(food.GetLocation(), point);
}