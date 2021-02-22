#include <agent_controller.h>

#include <iostream>

#include "game.h"
#include "keyboard_controller.h"
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

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  KeyboardController controller;
//  AgentController controller;
  Game game(kGridWidth, kGridHeight,
            std::make_unique<snake::GridSnake>(kGridWidth, kGridHeight));

  GameLoop(game, controller, renderer, kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSnake().Size() << "\n";
  return 0;
}