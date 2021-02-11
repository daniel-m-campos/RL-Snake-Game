#include "controller.h"

#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(snake::Snake &snake, snake::Direction input,
                                 snake::Direction opposite) const {
  if (snake.GetDirection() != opposite || snake.Size() == 1) {
    snake.SetDirection(input);
  }
}

void Controller::HandleInput(bool &running, snake::Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
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
}