#pragma once

#include <random>

#include "snake.h"

class Food
{
  public:
    Food() = delete;
    Food(std::size_t grid_width, std::size_t grid_height);
    Food(std::size_t grid_width, std::size_t grid_height,
         snake::Point<int> init_location);
    [[nodiscard]] auto get_location() const -> snake::Point<int> const &;
    auto try_feed(snake::Snake &snake) -> bool;

  private:
    snake::Point<int> _location{};
    std::random_device _dev;
    std::mt19937 _engine;
    std::uniform_int_distribution<int> _random_w;
    std::uniform_int_distribution<int> _random_h;

    void place_food(snake::Snake &snake);
    auto sample_location() -> snake::Point<int>;
};
