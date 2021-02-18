#ifndef RLSNAKEGAME_FOOD_H
#define RLSNAKEGAME_FOOD_H

#include <random>

#include "snake.h"

class Food {
 public:
  Food() = delete;
  Food(std::size_t grid_width, std::size_t grid_height);
  Food(std::size_t grid_width, std::size_t grid_height,
       snake::Point<int> init_location);
  [[nodiscard]] const snake::Point<int>& GetLocation() const;
  bool TryFeed(snake::Snake* snake);

 private:
  snake::Point<int> _location{};
  std::random_device _dev;
  std::mt19937 _engine;
  std::uniform_int_distribution<int> _random_w;
  std::uniform_int_distribution<int> _random_h;

  void PlaceFood(snake::Snake* snake);
  snake::Point<int> SampleLocation();
};

#endif  // RLSNAKEGAME_FOOD_H
