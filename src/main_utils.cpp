#include "main_utils.h"

#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <cstddef>
#include <iostream>
#include <memory>

#include "SDL.h" // IWYU pragma: keep
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "snake.h"

namespace
{
Uint32 constexpr milliseconds_per_second{1000};
float constexpr initial_speed{0.1F};
float constexpr speed_change{0.02F};
} // namespace

void game_loop(Game &game, Controller &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start{0};
    Uint32 frame_end{0};
    Uint32 frame_duration{0};
    int frame_count = 0;
    bool running    = true;

    while (running)
    {
        frame_start = SDL_GetTicks();

        running = controller.update(game);
        game.update();
        renderer.render(game.get_snake(), game.get_food());

        frame_end = SDL_GetTicks();

        frame_count++;
        frame_duration = frame_end - frame_start;

        if (frame_end - title_timestamp >= milliseconds_per_second)
        {
            renderer.update_window_title(game.get_score(), frame_count);
            frame_count     = 0;
            title_timestamp = frame_end;
        }

        if (frame_duration < target_frame_duration)
        {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }
}

void play(Controller &controller, std::size_t grid_width, std::size_t grid_height,
          std::size_t frames_per_second, std::size_t screen_width,
          std::size_t screen_height)
{
    std::size_t ms_per_frame{milliseconds_per_second / frames_per_second};

    Renderer renderer(screen_width, screen_height, grid_width, grid_height);
    Game game(grid_width, grid_height,
              std::make_unique<snake::GridSnake>(grid_width, grid_height),
              initial_speed, speed_change);

    game_loop(game, controller, renderer, ms_per_frame);

    print(game);
}

void print(Game const &game)
{
    std::cout << "Game has terminated successfully!\n";
    std::cout << "Score: " << game.get_score() << "\n";
    std::cout << "Size: " << game.get_snake().size() << "\n";
}
