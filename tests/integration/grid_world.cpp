#include "grid_world.h"

#include <unordered_map>
#include <vector>

void GridWorld::Update(Move action) {
  auto new_position = _position + action;
  if (_position == new_position) {
    _reward = -1.0;
    return;
  }
  if (_position.x == 1 && _position.y == 4) {
    _position.y = 0;
    _reward = 10.0;
    return;
  }
  if (_position.x == 3 && _position.y == 4) {
    _position.y = 2;
    _reward = 5.0;
    return;
  }
  _position = new_position;
  _reward = 0.0;
}

Position GridWorld::GetState() { return _position; }

double GridWorld::GetReward() { return _reward; }

std::unordered_map<Position, std::vector<Move>> CreateActionStateMap() {
  std::unordered_map<Position, std::vector<Move>> state_action_map;
  auto moves = {Move::kNorth, Move::kSouth, Move::kEast, Move::kWest};
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      state_action_map[{i, j}] = moves;
    }
  }
  return state_action_map;
}