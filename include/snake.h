#pragma once

#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <ostream>
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

[[nodiscard]] auto get_opposite(Direction const &) -> Direction;

inline std::array<Direction, 4> constexpr directions{
    Direction::kUp,
    Direction::kDown,
    Direction::kLeft,
    Direction::kRight,
};

template <typename T> struct Point
{
    T x;
    T y;
    auto operator<=>(Point const &) const        = default;
    auto operator==(Point const &) const -> bool = default;
};

class Snake
{
  public:
    Snake()                                                               = default;
    Snake(Snake const &)                                                  = delete;
    auto operator=(Snake const &) -> Snake &                              = delete;
    Snake(Snake &&)                                                       = default;
    auto operator=(Snake &&) -> Snake &                                   = default;
    virtual ~Snake()                                                      = default;
    virtual void update()                                                 = 0;
    virtual void grow_body()                                              = 0;
    [[nodiscard]] virtual auto snake_cell(int x, int y) const -> bool     = 0;
    [[nodiscard]] virtual auto size() const -> size_t                     = 0;
    [[nodiscard]] virtual auto get_direction() const -> Direction         = 0;
    virtual void set_direction(Direction direction)                       = 0;
    [[nodiscard]] virtual auto is_alive() const -> bool                   = 0;
    [[nodiscard]] virtual auto get_head_x() const -> float                = 0;
    [[nodiscard]] virtual auto get_head_y() const -> float                = 0;
    [[nodiscard]] virtual auto get_speed() const -> float                 = 0;
    virtual void set_speed(float speed)                                   = 0;
    [[nodiscard]] virtual std::vector<Point<int>> const &get_body() const = 0;
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
    [[nodiscard]] auto snake_cell(int x, int y) const -> bool override;
    [[nodiscard]] auto size() const -> size_t override;
    [[nodiscard]] auto get_direction() const -> Direction override;
    void set_direction(Direction direction) override;
    [[nodiscard]] auto is_alive() const -> bool override;
    [[nodiscard]] auto get_head_x() const -> float override;
    [[nodiscard]] auto get_head_y() const -> float override;
    [[nodiscard]] auto get_speed() const -> float override;
    void set_speed(float speed) override;
    [[nodiscard]] auto get_body() const -> std::vector<Point<int>> const & override;

  private:
    void update_head();
    void update_body(Point<int> const &current_cell, Point<int> const &prev_cell);

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
