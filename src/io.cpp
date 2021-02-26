#include "io.h"

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

std::vector<std::string> io::FindFiles() {
  std::vector<std::string> files;
  DIR* directory = opendir(".");
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    std::string filename(file->d_name);
    auto iter = filename.find("_action_valuer.txt");
    if (iter != std::string::npos) {
      files.push_back(filename);
    }
  }
  closedir(directory);
  return files;
}