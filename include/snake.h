#pragma once

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <set>
#include <tuple>
#include <vector>

namespace snake
{

enum class Direction : std::uint8_t
{
    kUp,
    kDown,
    kLeft,
    kRight,
};

auto operator<<(std::ostream &os, Direction action) -> std::ostream &;

auto get_opposite(Direction const &) -> Direction;

std::set<Direction> const directions{
    Direction::kUp,
    Direction::kDown,
    Direction::kLeft,
    Direction::kRight,
};

template <typename T> struct Point
{
    T x;
    T y;
    auto operator==(Point const &rhs) const -> bool
    {
        return std::tie(x, y) == std::tie(rhs.x, rhs.y);
    }
    auto operator!=(Point const &rhs) const -> bool
    {
        return rhs != *this;
    }
    auto operator<(Point const &rhs) const -> bool
    {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }
    auto operator>(Point const &rhs) const -> bool
    {
        return rhs < *this;
    }
    auto operator<=(Point const &rhs) const -> bool
    {
        return !(rhs < *this);
    }
    auto operator>=(Point const &rhs) const -> bool
    {
        return !(*this < rhs);
    }
};

class Snake
{
  public:
    Snake()                                                 = default;
    Snake(Snake const &)                                    = delete;
    auto operator=(Snake const &) -> Snake &                = delete;
    Snake(Snake &&)                                         = default;
    auto operator=(Snake &&) -> Snake &                     = default;
    virtual ~Snake()                                        = default;
    virtual void update()                                   = 0;
    virtual void grow_body()                                = 0;
    virtual auto snake_cell(int x, int y) const -> bool     = 0;
    virtual auto size() const -> size_t                     = 0;
    virtual auto get_direction() const -> Direction         = 0;
    virtual void set_direction(Direction direction)         = 0;
    virtual auto is_alive() const -> bool                   = 0;
    virtual auto get_head_x() const -> float                = 0;
    virtual auto get_head_y() const -> float                = 0;
    virtual auto get_speed() const -> float                 = 0;
    virtual void set_speed(float speed)                     = 0;
    virtual std::vector<Point<int>> const &get_body() const = 0;
};

inline float constexpr initial_snake_speed{0.1F};

class GridSnake : public Snake
{
  public:
    GridSnake(int grid_width, int grid_height)
        : _grid_width(grid_width), _grid_height(grid_height),
          _head_x(static_cast<float>(grid_width / 2)),
          _head_y(static_cast<float>(grid_height / 2))
    {
    }

    void update() override;
    void grow_body() override;
    auto snake_cell(int x, int y) const -> bool override;
    auto size() const -> size_t override;
    auto get_direction() const -> Direction override;
    void set_direction(Direction direction) override;
    auto is_alive() const -> bool override;
    auto get_head_x() const -> float override;
    auto get_head_y() const -> float override;
    auto get_speed() const -> float override;
    void set_speed(float speed) override;
    auto get_body() const -> std::vector<Point<int>> const & override;

  private:
    void update_head();
    void update_body(Point<int> &current_cell, Point<int> &prev_cell);

    bool _growing{false};
    int _grid_width;
    int _grid_height;
    Direction _direction = Direction::kUp;
    float _speed{initial_snake_speed};
    int _size{1};
    bool _alive{true};
    float _head_x;
    float _head_y;
    std::vector<Point<int>> _body;
};
} // namespace snake
