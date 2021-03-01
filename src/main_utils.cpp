#include "main_utils.h"

#include <iostream>

#include "action_valuer.h"
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "snake.h"
#include "state_action_map.h"

void GameLoop(Game& game, Controller& controller, Renderer& renderer,
              std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    running = controller.Update(game);
    game.Update();
    renderer.Render(game.GetSnake(), game.GetFood());

    frame_end = SDL_GetTicks();

    frame_count++;
    frame_duration = frame_end - frame_start;

    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(game.GetScore(), frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Play(Controller& controller, std::size_t grid_width,
          std::size_t grid_height, std::size_t frames_per_second,
          std::size_t screen_width, std::size_t screen_height) {
  std::size_t ms_per_frame{1000 / frames_per_second};

  Renderer renderer(screen_width, screen_height, grid_width, grid_height);
  Game game(grid_width, grid_height,
            std::make_unique<snake::GridSnake>(grid_width, grid_height), 0.1,
            0.02);

  GameLoop(game, controller, renderer, ms_per_frame);

  Print(game);
}

void Print(const Game& game) {
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSnake().Size() << "\n";
}
