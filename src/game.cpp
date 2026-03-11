#include <cstddef>
#include <memory>
#include <utility>

#include "food.h"
#include "game.h"
#include "snake.h"

Game::Game(std::size_t grid_width, std::size_t grid_height,
           std::unique_ptr<snake::Snake> snake, float initial_speed, float speed_change)
    : _speed_change{speed_change}, _snake{(std::move(snake))},
      _food{(std::make_unique<Food>(grid_width, grid_height))}
{
    _snake->set_speed(initial_speed);
}

Game::Game(std::unique_ptr<snake::Snake> snake, std::unique_ptr<Food> food,
           float speed_change)
    : _speed_change{speed_change}, _snake{(std::move(snake))}, _food{std::move(food)}
{
}

void Game::update()
{
    if (!_snake->is_alive())
    {
        return;
    }
    _snake->update();
    if (_food->try_feed(_snake.get()))
    {
        _score++;
        _snake->set_speed(_speed_change + _snake->get_speed());
    }
}

auto Game::get_score() const -> int
{
    return _score;
}

auto Game::get_snake() const -> snake::Snake &
{
    return *_snake;
}

auto Game::get_food() const -> Food &
{
    return *_food;
}
