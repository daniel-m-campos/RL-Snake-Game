#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "action_valuer.h"
#include "game_environment.h"
#include "snake.h"

namespace io
{

inline std::string const file_suffix{"_action_valuer.bin"};

void save(std::filesystem::path const &filepath,
          ActionValuer<GameState, snake::Direction> &action_valuer,
          bool only_argmax = true);

auto load(std::filesystem::path const &filepath)
    -> std::optional<std::unique_ptr<ActionValuer<GameState, snake::Direction>>>;

auto find_files(std::filesystem::path const &dir = std::filesystem::current_path())
    -> std::vector<std::string>;

} // namespace io
