#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

namespace snake {

enum class Direction { kUp, kDown, kLeft, kRight };

struct Point {
  int x;
  int y;
  bool operator==(const Point &rhs) const;
  bool operator!=(const Point &rhs) const;
};

class Snake {
 public:
  virtual ~Snake() = default;
  virtual void Update() = 0;
  virtual void GrowBody() = 0;
  virtual bool SnakeCell(int x, int y) const = 0;
  virtual size_t Size() const = 0;
  virtual Direction GetDirection() const = 0;
  virtual void SetDirection(Direction direction) = 0;
  virtual bool IsAlive() const = 0;
  virtual float GetHeadX() const = 0;
  virtual float GetHeadY() const = 0;
  virtual float GetSpeed() const = 0;
  virtual void SetSpeed(float speed) = 0;
  virtual const std::vector<Point> &GetBody() const = 0;
};

class GridSnake : public Snake {
 public:
  GridSnake(int grid_width, int grid_height)
      : _grid_width(grid_width),
        _grid_height(grid_height),
        _head_x(grid_width / 2),
        _head_y(grid_height / 2) {}

  void Update() override;
  void GrowBody() override;
  bool SnakeCell(int x, int y) const override;
  size_t Size() const override;
  enum Direction GetDirection() const override;
  void SetDirection(Direction direction) override;
  bool IsAlive() const override;
  float GetHeadX() const override;
  float GetHeadY() const override;
  float GetSpeed() const override;
  void SetSpeed(float speed) override;
  const std::vector<Point> &GetBody() const override;

 private:
  void UpdateHead();
  void UpdateBody(Point &current_cell, Point &prev_cell);

  bool _growing{false};
  int _grid_width;
  int _grid_height;
  enum Direction _direction = Direction::kUp;
  float _speed{0.1f};
  int _size{1};
  bool _alive{true};
  float _head_x;
  float _head_y;
  std::vector<Point> _body;
};
}  // namespace snake
#endif