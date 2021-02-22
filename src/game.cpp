#include "game.h"

#include <iostream>

Game::Game(std::size_t grid_width, std::size_t grid_height,
           std::unique_ptr<snake::Snake> snake, float initial_speed,
           float speed_change)
    : _snake{(std::move(snake))},
      _food{(std::make_unique<Food>(grid_width, grid_height))},
      _speed_change{speed_change} {
  _snake->SetSpeed(initial_speed);
}

Game::Game(std::unique_ptr<snake::Snake> snake, std::unique_ptr<Food> food,
           float speed_change)
    : _snake{(std::move(snake))},
      _food{std::move(food)},
      _speed_change{speed_change} {}

void Game::Update() {
  if (!_snake->IsAlive()) return;
  _snake->Update();
  if (_food->TryFeed(_snake.get())) {
    _score++;
    _snake->SetSpeed(_speed_change + _snake->GetSpeed());
  }
}

int Game::GetScore() const { return _score; }

snake::Snake &Game::GetSnake() const { return *_snake; }

Food &Game::GetFood() const { return *_food; }
