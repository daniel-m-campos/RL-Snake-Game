#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>

#include "food.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height,
       std::unique_ptr<snake::Snake> snake);
  Game(std::unique_ptr<snake::Snake> snake, std::unique_ptr<Food> food);
  [[nodiscard]] int GetScore() const;
  void Update();
  [[nodiscard]] snake::Snake& GetSnake() const;
  [[nodiscard]] Food& GetFood() const;

 private:
  int _score{0};
  std::unique_ptr<snake::Snake> _snake;
  std::unique_ptr<Food> _food;
};

#endif