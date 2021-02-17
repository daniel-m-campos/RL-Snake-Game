#ifndef RLSNAKEGAME_FOOD_H
#define RLSNAKEGAME_FOOD_H

#include <random>

#include "snake.h"

class Food {
 public:
  Food() = default;
  Food(std::size_t grid_width, std::size_t grid_height);
  Food(std::size_t grid_width, std::size_t grid_height,
       snake::Point init_location);
  snake::Point GetLocation();
  bool TryFeed(snake::Snake* snake);

 private:
  snake::Point _location{};
  std::random_device _dev;
  std::mt19937 _engine;
  std::uniform_int_distribution<int> _random_w;
  std::uniform_int_distribution<int> _random_h;

  void PlaceFood(snake::Snake* snake);
  snake::Point SampleLocation();
};

#endif  // RLSNAKEGAME_FOOD_H
