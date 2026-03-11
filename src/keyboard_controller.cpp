#include "keyboard_controller.h"

#include "SDL.h" // IWYU pragma: keep
#include "game.h"
#include "snake.h"
#include <SDL_events.h>
#include <SDL_keycode.h>

bool KeyboardController::update(Game &game)
{
    SDL_Event e;
    auto &snake = game.get_snake();
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
                snake.set_direction(snake::Direction::kUp);
                break;

            case SDLK_DOWN:
                snake.set_direction(snake::Direction::kDown);
                break;

            case SDLK_LEFT:
                snake.set_direction(snake::Direction::kLeft);
                break;

            case SDLK_RIGHT:
                snake.set_direction(snake::Direction::kRight);
                break;

            default:
                break;
            }
        }
    }
    return true;
}
