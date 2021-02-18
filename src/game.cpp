#include "game.h"

#include <iostream>

Game::Game(std::size_t grid_width, std::size_t grid_height,
           std::unique_ptr<snake::Snake> snake)
    : _snake{(std::move(snake))},
      _food{(std::make_unique<Food>(grid_width, grid_height))} {}

Game::Game(std::unique_ptr<snake::Snake> snake, std::unique_ptr<Food> food)
    : _snake{(std::move(snake))}, _food{std::move(food)} {}

void Game::Update() {
  if (!_snake->IsAlive()) return;
  _snake->Update();
  if (_food->TryFeed(_snake.get())) {
    _score++;
    _snake->SetSpeed(0.02f + _snake->GetSpeed());
  }
}

int Game::GetScore() const { return _score; }

snake::Snake &Game::GetSnake() const { return *_snake; }
Food &Game::GetFood() const { return *_food; }
