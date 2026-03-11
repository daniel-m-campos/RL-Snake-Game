#include "game_environment.h"

#include "gtest/gtest.h"

static std::size_t constexpr grid_width{32};
static std::size_t constexpr grid_height{32};

TEST(GameStateTest, TestComparisonOperators)
{
    GameState const first{{{0, 1}, {2, 3}}};
    GameState const second{{{4, 5}, {6, 7}}};
    EXPECT_EQ(first, first);
    EXPECT_EQ(second, second);
    EXPECT_FALSE(first == second);
    EXPECT_FALSE(second == first);
    EXPECT_TRUE(first < second);
    EXPECT_TRUE(first <= second);
    EXPECT_TRUE(second > first);
    EXPECT_TRUE(second >= first);
}

class GameEnvironmentInitializationFixture : public ::testing::Test
{
  public:
    snake::Point<int> init_location{10, 20};
    GameEnvironment environment{std::make_unique<Game>(
        std::make_unique<snake::GridSnake>(grid_width, grid_height),
        std::make_unique<Food>(grid_width, grid_height, init_location))};
};

TEST_F(GameEnvironmentInitializationFixture, TestInitialUpdate)
{
    snake::Point<int> expected_head_to_food{
        static_cast<int>(init_location.x - grid_width / 2),
        static_cast<int>(init_location.y - grid_height / 2)};
    EXPECT_EQ(environment.get_state().body_to_food[0], expected_head_to_food);
    environment.update(snake::Direction::kLeft);
    expected_head_to_food.x += 1;
    EXPECT_EQ(environment.get_state().body_to_food[0], expected_head_to_food);
}

TEST_F(GameEnvironmentInitializationFixture, TestGetRewardOnMovement)
{
    environment.update(snake::Direction::kRight);
    EXPECT_EQ(environment.get_reward(), -0.1);
}

class GameEnvironmentRewardsFixture : public ::testing::Test
{
  public:
    std::unique_ptr<snake::Snake> snake{
        std::make_unique<snake::GridSnake>(grid_width, grid_height)};
};

TEST_F(GameEnvironmentRewardsFixture, TestGetRewardOnDeath)
{
    snake->set_speed(1.0);
    for (int i = 0; i < 4; ++i)
    {
        snake->grow_body();
        snake->update();
    }
    EXPECT_EQ(snake->size(), 5);
    for (auto direction :
         {snake::Direction::kRight, snake::Direction::kDown, snake::Direction::kLeft})
    {
        snake->set_direction(direction);
        snake->update();
    }
    GameEnvironment environment{
        std::make_unique<Game>(kGridWidth, kGridHeight, std::move(snake))};
    environment.update(snake::Direction::kUp);
    EXPECT_EQ(environment.get_reward(), -100);
}

TEST_F(GameEnvironmentRewardsFixture, TestGetRewardOnEatFood)
{
    snake->set_speed(1.0);
    snake::Point<int> location = {static_cast<int>(snake->get_head_x()),
                                  -1 + static_cast<int>(snake->get_head_y())};
    auto food{std::make_unique<Food>(kGridWidth, kGridHeight, location)};
    auto game{std::make_unique<Game>(std::move(snake), std::move(food))};
    GameEnvironment environment{std::move(game)};
    environment.update(snake::Direction::kUp);
    EXPECT_EQ(environment.get_reward(), 1.0);
}
