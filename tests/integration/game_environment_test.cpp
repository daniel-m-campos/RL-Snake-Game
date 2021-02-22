#include "game_environment.h"

#include <iostream>

#include "action_valuer.h"
#include "agent_factory.h"
#include "game_simulator.h"
#include "gtest/gtest.h"
#include "io.h"
#include "state_action_map.h"

static constexpr std::size_t kGridWidth{16};
static constexpr std::size_t kGridHeight{16};

std::string FileName() {
  return std::to_string(kGridWidth) + "x" + std::to_string(kGridHeight) +
         "_action_valuer"
         ".txt";
}

class GameEnvironmentFixture : public ::testing::Test {
 public:
  std::set<snake::Direction> directions{snake::Directions};
  std::unique_ptr<SimpleStateActionMap<GameState, snake::Direction>>
      state_action_map = {
          std::make_unique<SimpleStateActionMap<GameState, snake::Direction>>(
              std::set<GameState>(), directions)};
  std::shared_ptr<SimpleActionValuer<GameState, snake::Direction>>
      action_valuer =
          std::make_shared<SimpleActionValuer<GameState, snake::Direction>>(
              std::move(state_action_map));
};

TEST_F(GameEnvironmentFixture, TestSimulationLoop) {
  auto agent = AgentFactory<GameState, snake::Direction>::CreateQAgent(
      action_valuer, 0.9, 0.9, 0.5, GameState{0, 0, 0, 0},
      snake::Direction::kUp);
  auto environment_factory = [&]() {
    auto snake = std::make_unique<snake::GridSnake>(kGridWidth, kGridHeight);
    snake->SetSpeed(1);
    auto game =
        std::make_unique<Game>(kGridWidth, kGridHeight, std::move(snake), 0);
    return GameEnvironment(std::move(game));
  };
  GameSimulator simulator{environment_factory, *agent};
  simulator.Simulate(100'000, 50'000);
  io::Save(FileName(), action_valuer.get());
}
