#include "io.h"

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iterator>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <sys/dirent.h>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "action_valuer_factory.h"
#include "game_environment.h"
#include "snake.h"

auto io::read_line(std::string &line) -> std::tuple<GameState, snake::Direction, double>
{
    std::replace(line.begin(), line.end(), ',', ' ');
    std::istringstream stream(line);
    double value{0.0};
    stream >> value;
    int direction_int{0};
    stream >> direction_int;
    int x{0};
    int y{0};
    std::vector<snake::Point<int>> body_to_food;
    while (stream >> x >> y)
    {
        body_to_food.push_back({x, y});
    }
    return std::make_tuple(GameState{body_to_food},
                           snake::Direction{static_cast<std::uint8_t>(direction_int)},
                           value);
}

template <>
auto io::load(std::string const &filename)
    -> std::unique_ptr<ActionValuer<GameState, snake::Direction>>
{
    std::ifstream file{filename};
    std::string line;
    if (file.is_open())
    {
        std::unordered_map<GameState, std::vector<snake::Direction>> state_action_dict;
        std::unordered_map<std::pair<GameState, snake::Direction>, double>
            action_value_dict;
        while (getline(file, line))
        {
            auto [state, direction, value] = read_line(line);
            state_action_dict[state].push_back(direction);
            action_value_dict[{state, direction}] = value;
        }
        std::set<GameState> states;
        std::transform(state_action_dict.begin(), state_action_dict.end(),
                       std::inserter(states, states.end()),
                       [](auto const &pair) { return pair.first; });

        auto action_valuer = ActionValuerFactory<GameState, snake::Direction>::
            create_simple_action_valuer(states, snake::directions);
        for (auto const &[state, actions] : state_action_dict)
        {
            for (auto const &action : actions)
            {
                action_valuer->set_value(state, action,
                                         action_value_dict[{state, action}]);
            }
        }
        return action_valuer;
    }
    return nullptr;
}

auto io::find_files() -> std::vector<std::string>
{
    std::vector<std::string> files;
    DIR *directory = opendir(".");
    struct dirent *file{nullptr};
    while ((file = readdir(directory)) != nullptr)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        std::string filename{file->d_name};
        auto iter = filename.find("_action_valuer.txt");
        if (iter != std::string::npos)
        {
            files.push_back(filename);
        }
    }
    closedir(directory);
    return files;
}
