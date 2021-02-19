#include "game_environment.h"

#include "../src/action_valuer.cpp"
#include "../src/agent.cpp"
#include "../src/learner.cpp"
#include "../src/policy.cpp"
#include "../src/rl_factory.cpp"
#include "../src/state_action_map.cpp"
#include "gtest/gtest.h"
#include "test_utils.h"

static constexpr std::size_t kGridWidth{10};
static constexpr std::size_t kGridHeight{10};

template <typename S, typename A>
void PrintAction(S& state, A& action) {
  std::unordered_map<A, std::string> map{{
      {snake::Direction::kUp, "^"},
      {snake::Direction::kDown, "v"},
      {snake::Direction::kLeft, "<"},
      {snake::Direction::kRight, ">"},
  }};

  std::cout << std::left << std::setw(4) << std::setfill(' ') << map[action]
            << "\t";
}

TEST(TestGameEnvironmentSimulation, TestSimulationLoop) {
  std::set<snake::Direction> directions{
      snake::Direction::kDown, snake::Direction::kUp, snake::Direction::kLeft,
      snake::Direction::kRight};
  auto state_action_map =
      std::make_unique<SimpleStateActionMap<GameState, snake::Direction>>(
          std::set<GameState>(), directions);
  auto action_valuer =
      std::make_shared<SimpleActionValuer<GameState, snake::Direction>>(
          std::move(state_action_map));

  int num_games = 10000;
  for (int n = 0; n < num_games; ++n) {
    auto agent = RLFactory<GameState, snake::Direction>::CreateQAgent(
        action_valuer,  0.9, 0.9, 0.5,
        GameState{0, 0, 0, 0}, snake::Direction::kUp);
    auto snake = std::make_unique<snake::GridSnake>(kGridWidth, kGridHeight);
    snake->SetSpeed(1);
    GameEnvironment environment{
        std::make_unique<Game>(kGridWidth, kGridHeight, std::move(snake), 0)};
    double reward = 42;
    int count = 0;
    while (reward != 0) {
      auto position = environment.GetState();
      auto action = agent->GetAction(position);
      environment.Update(action);
      reward = environment.GetReward();
      agent->Update(environment.GetState(), reward);
      ++count;
    }
    std::cout << "Simulation #" << n + 1 << ", Actions #" << count << "\n";
  }
  Print(action_valuer.get(), true, 10);
  Print(action_valuer.get(), false, 10);
}