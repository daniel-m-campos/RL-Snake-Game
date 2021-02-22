#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>

#include "food.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height,
       std::unique_ptr<snake::Snake> snake, float initial_speed = 1.0,
       float speed_change = 0.0);
  Game(std::unique_ptr<snake::Snake> snake, std::unique_ptr<Food> food,
       float speed_change = 0.0);
  [[nodiscard]] int GetScore() const;
  void Update();
  [[nodiscard]] snake::Snake& GetSnake() const;
  [[nodiscard]] Food& GetFood() const;

 private:
  int _score{0};
  float _speed_change;
  std::unique_ptr<snake::Snake> _snake;
  std::unique_ptr<Food> _food;
};

#endif