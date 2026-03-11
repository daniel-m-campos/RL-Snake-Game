#pragma once
#include <cstddef>

#include "controller.h"
#include "game.h"
#include "renderer.h"

namespace main_utils_defaults
{
std::size_t constexpr grid_width{32};
std::size_t constexpr grid_height{32};
std::size_t constexpr frames_per_second{60};
std::size_t constexpr screen_width{640};
std::size_t constexpr screen_height{640};
} // namespace main_utils_defaults

void game_loop(Game &game, Controller &controller, Renderer &renderer,
               std::size_t target_frame_duration);

void play(Controller &controller,
          std::size_t grid_width        = main_utils_defaults::grid_width,
          std::size_t grid_height       = main_utils_defaults::grid_height,
          std::size_t frames_per_second = main_utils_defaults::frames_per_second,
          std::size_t screen_width      = main_utils_defaults::screen_width,
          std::size_t screen_height     = main_utils_defaults::screen_height);

void print(Game const &game);
