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
  auto SquareDistance = [](const auto& point) -> int {
    return pow(point.x, 2) + pow(point.y, 2);
  };
  return SquareDistance(food) + SquareDistance(tail) <
         SquareDistance(rhs.food) + SquareDistance(rhs.tail);
}

GameEnvironment::GameEnvironment(std::unique_ptr<Game> game)
    : _game{std::move(game)} {
  UpdateGameState();
}

bool GameState::operator>(const GameState& rhs) const { return rhs < *this; }

bool GameState::operator<=(const GameState& rhs) const {
  return !(rhs < *this);
}

bool GameState::operator>=(const GameState& rhs) const {
  return !(*this < rhs);
}

GameState GameState::Create(Game& game) {
  auto& current_food = game.GetFood().GetLocation();
  snake::Point<int> new_food{
      static_cast<int>(current_food.x - game.GetSnake().GetHeadX()),
      static_cast<int>(current_food.y - game.GetSnake().GetHeadY())};
  snake::Point<int> new_tail = [&]() {
    if (game.GetSnake().GetBody().empty()) {
      return snake::Point<int>{0, 0};
    } else {
      auto& current_tail = game.GetSnake().GetBody().back();
      return snake::Point<int>{
          static_cast<int>(current_tail.x - game.GetSnake().GetHeadX()),
          static_cast<int>(current_tail.y - game.GetSnake().GetHeadY())};
    }
  }();
  return GameState{new_food, new_tail};
}

void GameEnvironment::Update(const snake::Direction& action) {
  if (!_alive) {
    _reward = 0;
    return;
  }
  auto score = _game->GetScore();
  _game->GetSnake().SetDirection(action);
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
