#ifndef RLSNAKEGAME_MAIN_UTILS_H
#define RLSNAKEGAME_MAIN_UTILS_H

#include "controller.h"
#include "game.h"
#include "renderer.h"

void GameLoop(Game& game, Controller& controller, Renderer& renderer,
              std::size_t target_frame_duration);

void Play(Controller& controller, std::size_t kGridWidth = 32,
          std::size_t kGridHeight = 32, std::size_t kFramesPerSecond = 60,
          std::size_t kScreenWidth = 640, std::size_t kScreenHeight = 640);

void Print(const Game& game);

#endif  // RLSNAKEGAME_MAIN_UTILS_H