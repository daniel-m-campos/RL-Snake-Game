#include "agent_controller.h"

#include <chrono>
#include <thread>

#include "SDL.h"
#include "agent_factory.h"
#include "game.h"
#include "game_environment.h"
#include "io.h"

AgentController::AgentController(const std::string& filename) {
  auto unique_valuer = io::Load<GameState, snake::Direction>(filename);
  std::shared_ptr<ActionValuer<GameState, snake::Direction>> action_valuer{
      std::move(unique_valuer)};
  _agent = AgentFactory<GameState, snake::Direction>::CreateQAgent(
      action_valuer, 0.01, 0.9, 0.5, GameState{{{8, 8}}},
      snake::Direction::kUp);
}

bool AgentController::Update(Game& game) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return false;
    }
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  auto state = GameState::Create(game);
  auto action = _agent->GetAction(state);
  snake::Snake& snake = game.GetSnake();
  snake.SetSpeed(1);
  snake.SetDirection(action);
  return snake.IsAlive();
}
