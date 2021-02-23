#ifndef RLSNAKEGAME_IO_H
#define RLSNAKEGAME_IO_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>

#include "action_valuer.h"
#include "action_valuer_factory.h"
#include "game_environment.h"

namespace io {

template <typename A, typename S>
void WriteLine(std::ofstream& file, ActionValuer<S, A>& action_valuer,
               const S& state, const A& action);

template <typename S, typename A>
void Save(const std::string& filename, ActionValuer<S, A>& action_valuer,
          int precision = 6, bool only_argmax = true) {
  std::ofstream file{filename, std::ios::trunc};
  if (file.is_open()) {
    file << std::setprecision(precision);
    StateActionMap<S, A>& state_action_map = action_valuer.GetStateActionMap();
    for (const auto& state : state_action_map.GetStates()) {
      if (only_argmax) {
        auto action = action_valuer.ArgMax(state)[0];
        WriteLine(file, action_valuer, state, action);
      } else {
        for (const auto& action : state_action_map.GetActions(state)) {
          WriteLine(file, action_valuer, state, action);
        }
      }
    }
  }
}

std::tuple<GameState, snake::Direction, double> ReadLine(std::string& line);

template <typename S, typename A>
std::unique_ptr<ActionValuer<S, A>> Load(const std::string& filename);

}  // namespace io

std::ostream& operator<<(std::ostream& os, const GameState& state) {
  for (const auto& part : state.body_to_food) {
    os << part.x << ",";
    os << part.y << ",";
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const snake::Direction action) {
  os << static_cast<int>(action);
  return os;
}

template <typename A, typename S>
void io::WriteLine(std::ofstream& file, ActionValuer<S, A>& action_valuer,
                   const S& state, const A& action) {
  auto value = action_valuer.GetValue(state, action);
  if (value == 0) {
    return;
  }
  file << value << ",";
  file << action << ",";
  file << state;
  file << "\n";
}

std::tuple<GameState, snake::Direction, double> io::ReadLine(
    std::string& line) {
  std::replace(line.begin(), line.end(), ',', ' ');
  std::istringstream stream(line);
  double value;
  stream >> value;
  int direction_int;
  stream >> direction_int;
  int x, y;
  std::vector<snake::Point<int>> body_to_food;
  while (stream >> x >> y) {
    body_to_food.push_back({x, y});
  }
  return std::make_tuple(GameState{body_to_food},
                         snake::Direction{direction_int}, value);
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
    while (getline(file, line)) {
      auto [state, direction, value] = ReadLine(line);
      if (value == 0) {
        continue;
      }
      state_action_dict[state].push_back(direction);
      action_value_dict[{state, direction}] = value;
    }
    std::set<GameState> states;
    std::transform(state_action_dict.begin(), state_action_dict.end(),
                   std::inserter(states, states.end()),
                   [](auto pair) { return pair.first; });

    auto action_valuer = ActionValuerFactory<GameState, snake::Direction>::
        CreateSimpleActionValuer(states, snake::Directions);
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
