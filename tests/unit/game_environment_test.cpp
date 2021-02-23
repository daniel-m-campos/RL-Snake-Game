#include "game_environment.h"

#include "gtest/gtest.h"

static constexpr std::size_t kGridWidth{32};
static constexpr std::size_t kGridHeight{32};

TEST(GameStateTest, TestComparisonOperators) {
  const GameState first{{{0, 1}, {2, 3}}};
  const GameState second{{{4, 5}, {6, 7}}};
  EXPECT_EQ(first, first);
  EXPECT_EQ(second, second);
  EXPECT_FALSE(first == second);
  EXPECT_FALSE(second == first);
  EXPECT_TRUE(first < second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(second > first);
  EXPECT_TRUE(second >= first);
}

class GameEnvironmentInitializationFixture : public ::testing::Test {
 public:
  snake::Point<int> init_location{10, 20};
  GameEnvironment environment{std::make_unique<Game>(
      std::make_unique<snake::GridSnake>(kGridWidth, kGridHeight),
      std::make_unique<Food>(kGridWidth, kGridHeight, init_location))};
};

TEST_F(GameEnvironmentInitializationFixture, TestInitialUpdate) {
  snake::Point<int> expected_head_to_food{
      static_cast<int>(init_location.x - kGridWidth / 2),
      static_cast<int>(init_location.y - kGridHeight / 2)};
  EXPECT_EQ(environment.GetState().body_to_food[0], expected_head_to_food);
  environment.Update(snake::Direction::kLeft);
  expected_head_to_food.x += 1;
  EXPECT_EQ(environment.GetState().body_to_food[0], expected_head_to_food);
}

TEST_F(GameEnvironmentInitializationFixture, TestGetRewardOnMovement) {
  environment.Update(snake::Direction::kRight);
  EXPECT_EQ(environment.GetReward(), -0.1);
}

class GameEnvironmentRewardsFixture : public ::testing::Test {
 public:
  std::unique_ptr<snake::Snake> snake{
      std::make_unique<snake::GridSnake>(kGridWidth, kGridHeight)};
};

TEST_F(GameEnvironmentRewardsFixture, TestGetRewardOnDeath) {
  snake->SetSpeed(1.0);
  for (int i = 0; i < 4; ++i) {
    snake->GrowBody();
    snake->Update();
  }
  EXPECT_EQ(snake->Size(), 5);
  for (auto direction : {snake::Direction::kRight, snake::Direction::kDown,
                         snake::Direction::kLeft}) {
    snake->SetDirection(direction);
    snake->Update();
  }
  GameEnvironment environment{
      std::make_unique<Game>(kGridWidth, kGridHeight, std::move(snake))};
  environment.Update(snake::Direction::kUp);
  EXPECT_EQ(environment.GetReward(), -100);
}

TEST_F(GameEnvironmentRewardsFixture, TestGetRewardOnEatFood) {
  snake->SetSpeed(1.0);
  snake::Point<int> location = {static_cast<int>(snake->GetHeadX()),
                                -1 + static_cast<int>(snake->GetHeadY())};
  auto food{std::make_unique<Food>(kGridWidth, kGridHeight, location)};
  auto game{std::make_unique<Game>(std::move(snake), std::move(food))};
  GameEnvironment environment{std::move(game)};
  environment.Update(snake::Direction::kUp);
  EXPECT_EQ(environment.GetReward(), 1.0);
}