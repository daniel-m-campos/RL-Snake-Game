#ifndef RLSNAKEGAME_IO_H
#define RLSNAKEGAME_IO_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>

#include "action_valuer.h"
#include "game_environment.h"
#include "rl_factory.h"

namespace io {

template <typename S, typename A>
void Save(const std::string& filename, ActionValuer<S, A>* action_valuer,
          int precision = 6) {
  std::ofstream file{filename, std::ios::trunc};
  if (file.is_open()) {
    file << std::setprecision(precision);
    StateActionMap<S, A>& state_action_map = action_valuer->GetStateActionMap();
    for (const auto& state : state_action_map.GetStates()) {
      for (const auto& action : state_action_map.GetActions(state)) {
        file << state << ",";
        file << action << ",";
        file << action_valuer->GetValue(state, action);
        file << "\n";
      }
    }
  }
}

template <typename S, typename A>
std::unique_ptr<ActionValuer<S, A>> Load(const std::string& filename);

}  // namespace io

std::ostream& operator<<(std::ostream& os, const GameState& state) {
  os << state.food.x << "," << state.food.y << ",";
  os << state.tail.x << "," << state.tail.y;
  return os;
}

std::ostream& operator<<(std::ostream& os, const snake::Direction action) {
  os << static_cast<int>(action);
  return os;
}

template <>
std::unique_ptr<ActionValuer<GameState, snake::Direction>> io::Load(
    const std::string& filename) {
  std::ifstream file{filename};
  std::string line;
  if (file.is_open()) {
    std::unordered_map<GameState, std::vector<snake::Direction>>
        state_action_dict;
    std::unordered_map<std::pair<GameState, snake::Direction>, double>
        action_value_dict;
    int food_x, food_y, tail_x, tail_y;
    int direction_int;
    double value;
    while (getline(file, line)) {
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream stream(line);
      if (stream >> food_x >> food_y >> tail_x >> tail_y >> direction_int >>
          value) {
        auto state = GameState{food_x, food_y, tail_x, tail_y};
        auto direction = snake::Direction{direction_int};
        state_action_dict[state].push_back(direction);
        action_value_dict[{state, direction}] = value;
      }
    }

    auto action_valuer =
        RLFactory<GameState, snake::Direction>::CreateSimpleActionValuer(
            state_action_dict);
    for (const auto& [state, actions] : state_action_dict) {
      for (const auto& action : actions) {
        action_valuer->SetValue(state, action,
                                action_value_dict[{state, action}]);
      }
    }
    return action_valuer;
  }
  return nullptr;
}

#endif  // RLSNAKEGAME_IO_H
