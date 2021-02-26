#include "main_utils.h"

#include <iostream>

#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "snake.h"

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

void Play(Controller& controller, std::size_t kGridWidth,
          std::size_t kGridHeight, std::size_t kFramesPerSecond,
          std::size_t kScreenWidth, std::size_t kScreenHeight) {
  std::size_t kMsPerFrame{1000 / kFramesPerSecond};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Game game(kGridWidth, kGridHeight,
            std::make_unique<snake::GridSnake>(kGridWidth, kGridHeight), 0.1,
            0.02);

  GameLoop(game, controller, renderer, kMsPerFrame);

  Print(game);
}

void Print(const Game& game) {
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSnake().Size() << "\n";
}
