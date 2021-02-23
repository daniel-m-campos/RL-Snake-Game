#include "agent_controller.h"

#include <io.h>

#include <chrono>
#include <thread>

#include "agent_factory.h"

const std::string AgentController::_filename{"16x16_action_valuer.txt"};

AgentController::AgentController() {
  auto unique_valuer = io::Load<GameState, snake::Direction>(_filename);
  std::shared_ptr<ActionValuer<GameState, snake::Direction>> action_valuer{
      std::move(unique_valuer)};
  _agent = AgentFactory<GameState, snake::Direction>::CreateQAgent(
      action_valuer, 0.01, 0.9, 0.5, GameState{{{8, 8}}},
      snake::Direction::kUp);
}

bool AgentController::Update(Game& game) {
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  auto state = GameState::Create(game);
  auto action = _agent->GetAction(state);
  snake::Snake& snake = game.GetSnake();
  snake.SetSpeed(1);
  snake.SetDirection(action);
  return snake.IsAlive();
}
