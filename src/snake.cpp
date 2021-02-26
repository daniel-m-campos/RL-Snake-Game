#include "snake.h"

#include <cmath>
#include <ostream>

using namespace snake;

std::ostream &snake::operator<<(std::ostream &os, Direction action) {
  os << static_cast<int>(action);
  return os;
}

Direction snake::GetOpposite(const Direction &direction) {
  switch (direction) {
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

void GridSnake::Update() {
  Point<int> prev_cell{
      static_cast<int>(_head_x),
      static_cast<int>(
          _head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  Point<int> current_cell{
      static_cast<int>(_head_x),
      static_cast<int>(_head_y)};  // Capture the head's cell after updating.

  // Update all of the _body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void GridSnake::UpdateHead() {
  switch (_direction) {
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
  _head_x = fmod(_head_x + _grid_width, _grid_width);
  _head_y = fmod(_head_y + _grid_height, _grid_height);
}

void GridSnake::UpdateBody(Point<int> &current_head_cell,
                           Point<int> &prev_head_cell) {
  // Add previous head location to vector
  _body.push_back(prev_head_cell);

  if (!_growing) {
    // Remove the tail from the vector.
    _body.erase(_body.begin());
  } else {
    _growing = false;
    _size++;
  }

  // Check if the snake has died.
  for (auto const &item : _body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      _alive = false;
    }
  }
}

void GridSnake::GrowBody() { _growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool GridSnake::SnakeCell(int x, int y) const {
  if (x == static_cast<int>(_head_x) && y == static_cast<int>(_head_y)) {
    return true;
  }
  for (auto const &item : _body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

size_t GridSnake::Size() const { return _size; }

enum Direction GridSnake::GetDirection() const { return _direction; }

void GridSnake::SetDirection(Direction direction) {
  if (direction != GetOpposite(_direction) || Size() == 1) {
    _direction = direction;
  }
}

bool GridSnake::IsAlive() const { return _alive; }

float GridSnake::GetHeadX() const { return _head_x; }

float GridSnake::GetHeadY() const { return _head_y; }

const std::vector<Point<int>> &GridSnake::GetBody() const { return _body; }

float GridSnake::GetSpeed() const { return _speed; }

void GridSnake::SetSpeed(float speed) { _speed = speed; }
