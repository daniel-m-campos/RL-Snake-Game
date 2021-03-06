#ifndef RLSNAKEGAME_GRIDWORLD_H
#define RLSNAKEGAME_GRIDWORLD_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "environment.h"
#include "hash_util.h"
#include "state_action_map.h"

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
      default:
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

std::unique_ptr<StateActionMap<Position, Move>> CreateActionStateMap();

class GridWorld : public Environment<Position, Move> {
 public:
  void Update(const Move& action) override;
  const Position& GetState() override;
  double GetReward() override;
  bool HasTerminated() override;

 private:
  Position _position;
  double _reward{0};
};

#endif  // RLSNAKEGAME_GRIDWORLD_H
