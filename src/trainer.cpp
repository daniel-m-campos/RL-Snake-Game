#include "trainer.h"

#include "action_valuer.h"
#include "action_valuer_factory.h"
#include "agent_factory.h"
#include "game.h"
#include "game_simulator.h"
#include "io.h"
#include "snake.h"
#include "state_action_map.h"

void Train(std::size_t grid_width, std::size_t grid_height, long num_episodes,
           long max_steps, double epsilon, double discount_factor,
           double step_size) {
  std::set<GameState> states;
  std::shared_ptr<ActionValuer<GameState, snake::Direction>> action_valuer =
      ActionValuerFactory<GameState, snake::Direction>::
          CreateSimpleActionValuer(states, snake::Directions);
  auto agent = AgentFactory<GameState, snake::Direction>::CreateQAgent(
      action_valuer, epsilon, discount_factor, step_size, GameState{{{0, 0}}},
      snake::Direction::kUp);

  auto environment_factory = [&]() {
    auto snake = std::make_unique<snake::GridSnake>(grid_width, grid_height);
    snake->SetSpeed(1);
    auto game =
        std::make_unique<Game>(grid_width, grid_height, std::move(snake), 0);
    return GameEnvironment(std::move(game));
  };

  GameSimulator simulator{environment_factory, *agent};
  simulator.Simulate(num_episodes, max_steps);

  io::Save(FileName(grid_width, grid_height), *action_valuer);
}

std::string FileName(std::size_t grid_width, std::size_t grid_height) {
  return std::to_string(grid_width) + "x" + std::to_string(grid_height) +
         "_action_valuer"
         ".txt";
}