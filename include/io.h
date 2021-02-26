#ifndef RLSNAKEGAME_IO_H
#define RLSNAKEGAME_IO_H

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>

#include "action_valuer.h"
#include "action_valuer_factory.h"
#include "game_environment.h"
#include "snake.h"

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

std::vector<std::string> FindFiles();

}  // namespace io

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

#endif  // RLSNAKEGAME_IO_H
