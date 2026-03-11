#include "snake.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <ostream>

using namespace snake; // NOLINT(google-build-using-namespace)

auto snake::operator<<(std::ostream &os, Direction action) -> std::ostream &
{
    os << static_cast<int>(action);
    return os;
}

auto snake::get_opposite(Direction const &direction) -> Direction
{
    switch (direction)
    {
    case snake::Direction::kDown:
        return snake::Direction::kUp;
    case snake::Direction::kUp:
        return snake::Direction::kDown;
    case snake::Direction::kLeft:
        return snake::Direction::kRight;
    default:
        return snake::Direction::kLeft;
    }
}

void GridSnake::update()
{
    Point<int> prev_cell{
        static_cast<int>(_head_x),
        static_cast<int>(_head_y)}; // We first capture the head's cell before updating.
    update_head();
    Point<int> current_cell{
        static_cast<int>(_head_x),
        static_cast<int>(_head_y)}; // Capture the head's cell after updating.

    // Update all of the _body vector items if the snake head has moved to a new
    // cell.
    if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y)
    {
        update_body(current_cell, prev_cell);
    }
}

void GridSnake::update_head()
{
    switch (_direction)
    {
    case Direction::kUp:
        _head_y -= _speed;
        break;

    case Direction::kDown:
        _head_y += _speed;
        break;

    case Direction::kLeft:
        _head_x -= _speed;
        break;

    case Direction::kRight:
        _head_x += _speed;
        break;
    }

    // Wrap the GridSnake around to the beginning if going off of the screen.
    _head_x = fmodf(_head_x + static_cast<float>(_grid_width),
                    static_cast<float>(_grid_width));
    _head_y = fmodf(_head_y + static_cast<float>(_grid_height),
                    static_cast<float>(_grid_height));
}

void GridSnake::update_body(Point<int> &current_cell, Point<int> &prev_cell)
{
    // Add previous head location to vector
    _body.push_back(prev_cell);

    if (!_growing)
    {
        // Remove the tail from the vector.
        _body.erase(_body.begin());
    }
    else
    {
        _growing = false;
        _size++;
    }

    // Check if the snake has died.
    _alive =
        !std::any_of(_body.begin(), _body.end(), [&current_cell](Point<int> const &item)
                     { return current_cell.x == item.x && current_cell.y == item.y; });
}

void GridSnake::grow_body()
{
    _growing = true;
}

// Inefficient method to check if cell is occupied by snake.
bool GridSnake::snake_cell(int x, int y) const
{
    if (x == static_cast<int>(_head_x) && y == static_cast<int>(_head_y))
    {
        return true;
    }
    return std::any_of(_body.begin(), _body.end(), [x, y](Point<int> const &item)
                       { return x == item.x && y == item.y; });
}

size_t GridSnake::size() const
{
    return _size;
}

Direction GridSnake::get_direction() const
{
    return _direction;
}

void GridSnake::set_direction(Direction direction)
{
    if (direction != get_opposite(_direction) || size() == 1)
    {
        _direction = direction;
    }
}

bool GridSnake::is_alive() const
{
    return _alive;
}

float GridSnake::get_head_x() const
{
    return _head_x;
}

float GridSnake::get_head_y() const
{
    return _head_y;
}

std::vector<Point<int>> const &GridSnake::get_body() const
{
    return _body;
}

auto GridSnake::get_speed() const -> float
{
    return _speed;
}

void GridSnake::set_speed(float speed)
{
    _speed = speed;
}
