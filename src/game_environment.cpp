#include "game_environment.h"

#include <cmath>
#include <tuple>

#include "game.h"

GameEnvironment::GameEnvironment(std::unique_ptr<Game> game)
    : _game{std::move(game)} {
  UpdateGameState();
}

bool GameState::operator==(const GameState& rhs) const {
  return std::tie(body_to_food) == std::tie(rhs.body_to_food);
}

bool GameState::operator!=(const GameState& rhs) const {
  return !(rhs == *this);
}

bool GameState::operator<(const GameState& rhs) const {
  return std::tie(body_to_food) < std::tie(rhs.body_to_food);
}

bool GameState::operator>(const GameState& rhs) const { return rhs < *this; }

bool GameState::operator<=(const GameState& rhs) const {
  return !(rhs < *this);
}

bool GameState::operator>=(const GameState& rhs) const {
  return !(*this < rhs);
}

GameState GameState::Create(Game& game) {
  auto food = game.GetFood().GetLocation();
  std::vector<snake::Point<int>> body_to_food;
  body_to_food.push_back(
      {static_cast<int>(food.x - game.GetSnake().GetHeadX()),
       static_cast<int>(food.y - game.GetSnake().GetHeadY())});
  for (const auto& part : game.GetSnake().GetBody()) {
    body_to_food.push_back({food.x - part.x, food.y - part.y});
  }
  return GameState{body_to_food};
}

void GameEnvironment::Update(const snake::Direction& action) {
  if (!_alive) {
    _reward = 0;
    return;
  }
  auto score = _game->GetScore();
  _game->GetSnake().SetDirection(action);
  _game->GetSnake().SetSpeed(1);
  _game->Update();
  UpdateGameState();
  if (!_game->GetSnake().IsAlive()) {
    _alive = false;
    _reward = -100;
  } else if (_game->GetScore() > score) {
    _reward = 1;
  } else {
    _reward = -0.1;
  }
}

void GameEnvironment::UpdateGameState() {
  auto& food = _game->GetFood().GetLocation();
  _state = GameState::Create(*_game);
}

const GameState& GameEnvironment::GetState() { return _state; }

double GameEnvironment::GetReward() { return _reward; }

bool GameEnvironment::HasTerminated() { return !_alive; }
