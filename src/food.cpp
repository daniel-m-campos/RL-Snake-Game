#include "food.h"

#include <iostream>

Food::Food(std::size_t grid_width, std::size_t grid_height)
    : _engine{(_dev())},
      _random_w{0, static_cast<int>(grid_width)},
      _random_h{0, static_cast<int>(grid_height)} {
  _location = SampleLocation();
}

Food::Food(std::size_t grid_width, std::size_t grid_height,
           snake::Point init_location)
    : _location{init_location},
      _engine{(_dev())},
      _random_w{(0, static_cast<int>(grid_width))},
      _random_h{(0, static_cast<int>(grid_height))} {}

const snake::Point& Food::GetLocation() const { return _location; }

bool Food::TryFeed(snake::Snake* snake) {
  int new_x = static_cast<int>(snake->GetHeadX());
  int new_y = static_cast<int>(snake->GetHeadY());
  if (_location.x == new_x && _location.y == new_y) {
    snake->GrowBody();
    PlaceFood(snake);
    return true;
  }
  return false;
}

void Food::PlaceFood(snake::Snake* snake) {
  while (true) {
    auto new_location = SampleLocation();
    if (!snake->SnakeCell(new_location.x, new_location.y)) {
      _location = new_location;
      return;
    }
  }
}

snake::Point Food::SampleLocation() {
  return snake::Point{_random_w(_engine), _random_h(_engine)};
}
