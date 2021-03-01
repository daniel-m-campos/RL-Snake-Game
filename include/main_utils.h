#ifndef RLSNAKEGAME_MAIN_UTILS_H
#define RLSNAKEGAME_MAIN_UTILS_H

#include "controller.h"
#include "game.h"
#include "renderer.h"

void GameLoop(Game& game, Controller& controller, Renderer& renderer,
              std::size_t target_frame_duration);

void Play(Controller& controller, std::size_t grid_width = 32,
          std::size_t grid_height = 32, std::size_t frames_per_second = 60,
          std::size_t screen_width = 640, std::size_t screen_height = 640);

void Print(const Game& game);

#endif  // RLSNAKEGAME_MAIN_UTILS_H