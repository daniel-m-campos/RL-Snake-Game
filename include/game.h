#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>

#include "SDL.h"
#include "controller.h"
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
  std::unique_ptr<snake::Snake> _snake;
  SDL_Point _food;

  std::random_device _dev;
  std::mt19937 _engine;
  std::uniform_int_distribution<int> _random_w;
  std::uniform_int_distribution<int> _random_h;

  int _score{0};

  void PlaceFood();
  void Update();
};

#endif