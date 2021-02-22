#include "game_environment.h"

#include "gtest/gtest.h"

static constexpr std::size_t kGridWidth{32};
static constexpr std::size_t kGridHeight{32};

TEST(GameStateTest, TestComparisonOperators) {
  const GameState first{0, 1, 2, 3};
  const GameState second{4, 5, 6, 7};
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
  GameEnvironment environment{std::make_unique<Game>(
      kGridWidth, kGridHeight,
      std::make_unique<snake::GridSnake>(kGridWidth, kGridHeight))};
};

TEST_F(GameEnvironmentInitializationFixture, TestInitialUpdate) {
  snake::Point<int> expected_tail{kGridWidth / 2, kGridHeight / 2};
  EXPECT_EQ(environment.GetState().snake_head, expected_tail);
  environment.Update(snake::Direction::kRight);
  expected_tail.x += 1;
  EXPECT_EQ(environment.GetState().snake_head, expected_tail);
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