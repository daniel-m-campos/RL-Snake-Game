#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>

#include "controller.h"
#include "food.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height,
       std::unique_ptr<snake::Snake> snake);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  int _score{0};
  std::unique_ptr<snake::Snake> _snake;
  Food _food;

  void Update();
};

#endif