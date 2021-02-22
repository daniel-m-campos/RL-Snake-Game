#include "keyboard_controller.h"

#include "SDL.h"
#include "game.h"

void KeyboardController::ChangeDirection(snake::Snake& snake,
                                         snake::Direction input,
                                         snake::Direction opposite) {
  if (snake.GetDirection() != opposite || snake.Size() == 1) {
    snake.SetDirection(input);
  }
}

bool KeyboardController::Update(Game& game) {
  SDL_Event e;
  auto& snake = game.GetSnake();
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, snake::Direction::kUp,
                          snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, snake::Direction::kDown,
                          snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, snake::Direction::kLeft,
                          snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, snake::Direction::kRight,
                          snake::Direction::kLeft);
          break;
      }
    }
  }
  return true;
}