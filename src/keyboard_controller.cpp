#include "keyboard_controller.h"

#include "SDL.h"
#include "game.h"

bool KeyboardController::Update(Game& game) {
  SDL_Event e;
  auto& snake = game.GetSnake();
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          snake.SetDirection(snake::Direction::kUp);
          break;

        case SDLK_DOWN:
          snake.SetDirection(snake::Direction::kDown);
          break;

        case SDLK_LEFT:
          snake.SetDirection(snake::Direction::kLeft);
          break;

        case SDLK_RIGHT:
          snake.SetDirection(snake::Direction::kRight);
          break;
      }
    }
  }
  return true;
}