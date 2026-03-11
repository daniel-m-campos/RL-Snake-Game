#include "food.h"
#include <cstddef>

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
Food::Food(std::size_t grid_width, std::size_t grid_height)
    : _engine{(_dev())}, _random_w{0, static_cast<int>(grid_width) - 1},
      _random_h{0, static_cast<int>(grid_height) - 1}
{
    _location = sample_location();
}

Food::Food(std::size_t grid_width, std::size_t grid_height,
           snake::Point<int> init_location)
    : _location{init_location}, _engine{(_dev())},
      _random_w{0, static_cast<int>(grid_width)},
      _random_h{0, static_cast<int>(grid_height)}
{
}
// NOLINTEND(bugprone-easily-swappable-parameters)

snake::Point<int> const &Food::get_location() const
{
    return _location;
}

auto Food::try_feed(snake::Snake *snake) -> bool
{
    int new_x = static_cast<int>(snake->get_head_x());
    int new_y = static_cast<int>(snake->get_head_y());
    if (_location.x == new_x && _location.y == new_y)
    {
        snake->grow_body();
        place_food(snake);
        return true;
    }
    return false;
}

void Food::place_food(snake::Snake *snake)
{
    while (true)
    {
        auto new_location = sample_location();
        if (!snake->snake_cell(new_location.x, new_location.y))
        {
            _location = new_location;
            return;
        }
    }
}

auto Food::sample_location() -> snake::Point<int>
{
    return snake::Point<int>{_random_w(_engine), _random_h(_engine)};
}
