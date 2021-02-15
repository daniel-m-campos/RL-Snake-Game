#ifndef RLSNAKEGAME_GRID_WORLD_H
#define RLSNAKEGAME_GRID_WORLD_H

#include <unordered_map>
#include <vector>

#include "environment.h"
#include "hash_util.h"

enum class Move {
  kNorth,
  kSouth,
  kEast,
  kWest,
};

struct Position {
  int x{0};
  int y{0};

  Position operator+(const Move& action) {
    switch (action) {
      case Move::kNorth:
        return y < 4 ? Position{x, y + 1} : *this;
      case Move::kSouth:
        return y > 0 ? Position{x, y - 1} : *this;
      case Move::kEast:
        return x < 4 ? Position{x + 1, y} : *this;
      case Move::kWest:
        return x > 0 ? Position{x - 1, y} : *this;
    }
  }

  Position operator+=(const Move& action) { return *this + action; }

  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }

  bool operator<(const Position& other) const { return x < other.x; }
};

template <>
struct std::hash<Position> {
  inline size_t operator()(const Position& position) const {
    size_t seed = 0;
    ::hash_combine(seed, position.x);
    ::hash_combine(seed, position.y);
    return seed;
  }
};

std::unordered_map<Position, std::vector<Move>> CreateActionStateMap();

class GridWorld : public Environment<Position, Move> {
 public:
  void Update(Move action) override;
  Position GetState() override;
  double GetReward() override;

 private:
  Position _position;
  double _reward{0};
};

#endif  // RLSNAKEGAME_GRID_WORLD_H
