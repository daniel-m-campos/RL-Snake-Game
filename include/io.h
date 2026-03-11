#pragma once

#include <dirent.h>
#include <ios>
#include <memory>
#include <tuple>
#include <unistd.h>

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "action_valuer.h"
#include "game_environment.h"
#include "snake.h"
#include "state_action_map.h"

namespace io
{

int constexpr default_precision = 6;

template <typename A, typename S>
void write_line(std::ofstream &file, ActionValuer<S, A> &action_valuer, S const &state,
                A const &action);

template <typename S, typename A>
void save(std::string const &filename, ActionValuer<S, A> &action_valuer,
          int precision = default_precision, bool only_argmax = true)
{
    std::ofstream file{filename, std::ios::trunc};
    if (file.is_open())
    {
        file << std::setprecision(precision);
        StateActionMap<S, A> &state_action_map = action_valuer.get_state_action_map();
        for (auto const &state : state_action_map.get_states())
        {
            if (only_argmax)
            {
                auto action = action_valuer.arg_max(state)[0];
                write_line(file, action_valuer, state, action);
            }
            else
            {
                for (auto const &action : state_action_map.get_actions(state))
                {
                    write_line(file, action_valuer, state, action);
                }
            }
        }
    }
}

auto read_line(std::string &line) -> std::tuple<GameState, snake::Direction, double>;

template <typename S, typename A>
auto load(std::string const &filename) -> std::unique_ptr<ActionValuer<S, A>>;

auto find_files() -> std::vector<std::string>;

} // namespace io

template <typename A, typename S>
void io::write_line(std::ofstream &file, ActionValuer<S, A> &action_valuer,
                    S const &state, A const &action)
{
    auto value = action_valuer.get_value(state, action);
    if (value == 0)
    {
        return;
    }
    file << value << ",";
    file << action << ",";
    file << state;
    file << "\n";
}
