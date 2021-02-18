#include "game_environment.h"

#include <cmath>
#include <tuple>

#include "game.h"

bool GameState::operator==(const GameState& rhs) const {
  return std::tie(food, tail) == std::tie(rhs.food, rhs.tail);
}

bool GameState::operator!=(const GameState& rhs) const {
  return !(rhs == *this);
}

bool GameState::operator<(const GameState& rhs) const {
  auto SquareDistance = [](const auto& point) -> double {
    return pow(point.x, 2) + pow(point.y, 2);
  };
  return SquareDistance(food) + SquareDistance(tail) <
         SquareDistance(rhs.food) + SquareDistance(rhs.tail);
}

GameEnvironment::GameEnvironment(std::unique_ptr<Game> game) : _game{std::move(game)} {
  UpdateGameState();
}

bool GameState::operator>(const GameState& rhs) const { return rhs < *this; }

bool GameState::operator<=(const GameState& rhs) const {
  return !(rhs < *this);
}

bool GameState::operator>=(const GameState& rhs) const {
  return !(*this < rhs);
}

void GameEnvironment::Update(snake::Direction action) {
  auto score = _game->GetScore();
  _game->GetSnake().SetDirection(action);
  _game->Update();
  UpdateGameState();
  if (!_game->GetSnake().IsAlive()) {
    _reward = -100;
  } else if (_game->GetScore() > score) {
    _reward = 1;
  } else {
    _reward = -0.1;
  }
}

void GameEnvironment::UpdateGameState() {
  auto& food = _game->GetFood().GetLocation();
  _state.food.x = static_cast<double>(food.x - _game->GetSnake().GetHeadX());
  _state.food.y = static_cast<double>(food.y - _game->GetSnake().GetHeadY());
  if (_game->GetSnake().GetBody().empty()) {
    _state.tail.x = 0.0;
    _state.tail.y = 0.0;
  } else {
    auto& tail = _game->GetSnake().GetBody().back();
    _state.tail.x = static_cast<double>(tail.x - _game->GetSnake().GetHeadX());
    _state.tail.y = static_cast<double>(tail.y - _game->GetSnake().GetHeadY());
  }
}

GameState GameEnvironment::GetState() { return _state; }

double GameEnvironment::GetReward() { return _reward; }
