#include "renderer.h"

#include <SDL_error.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>
#include <cstddef>
#include <iostream>
#include <string>

#include "SDL.h"
#include "food.h"
#include "snake.h"

namespace
{
Uint8 constexpr bg_r{0x1E};
Uint8 constexpr bg_g{0x1E};
Uint8 constexpr bg_b{0x1E};
Uint8 constexpr food_r{0xFF};
Uint8 constexpr food_g{0xCC};
Uint8 constexpr body_r{0xFF};
Uint8 constexpr body_g{0xFF};
Uint8 constexpr body_b{0xFF};
Uint8 constexpr head_alive_r{0x00};
Uint8 constexpr head_alive_g{0x7A};
Uint8 constexpr head_alive_b{0xCC};
Uint8 constexpr head_dead_r{0xFF};
Uint8 constexpr alpha_full{0xFF};
} // namespace

Renderer::Renderer(std::size_t const screen_width, std::size_t const screen_height,
                   std::size_t const grid_width, std::size_t const grid_height)
    : screen_width(screen_width), screen_height(screen_height), grid_width(grid_width),
      grid_height(grid_height)
{
    if (!_sdl_guard._owns)
    {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create Window
    _sdl_window.reset(
        SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         static_cast<int>(screen_width),
                         static_cast<int>(screen_height), SDL_WINDOW_SHOWN));

    if (_sdl_window == nullptr)
    {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create renderer
    _sdl_renderer.reset(
        SDL_CreateRenderer(_sdl_window.get(), -1, SDL_RENDERER_ACCELERATED));
    if (_sdl_renderer == nullptr)
    {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
}

void Renderer::render(snake::Snake const &snake, Food const &food)
{
    SDL_Rect block;
    block.w = static_cast<int>(screen_width / grid_width);
    block.h = static_cast<int>(screen_height / grid_height);

    // Clear screen
    SDL_SetRenderDrawColor(_sdl_renderer.get(), bg_r, bg_g, bg_b, alpha_full);
    SDL_RenderClear(_sdl_renderer.get());

    // Render _food
    SDL_SetRenderDrawColor(_sdl_renderer.get(), food_r, food_g, 0x00, alpha_full);
    auto location = food.get_location();
    block.x       = location.x * block.w;
    block.y       = location.y * block.h;
    SDL_RenderFillRect(_sdl_renderer.get(), &block);

    // Render snake's _body
    SDL_SetRenderDrawColor(_sdl_renderer.get(), body_r, body_g, body_b, alpha_full);
    for (auto const &point : snake.get_body())
    {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(_sdl_renderer.get(), &block);
    }

    // Render snake's head
    block.x = static_cast<int>(snake.get_head_x()) * block.w;
    block.y = static_cast<int>(snake.get_head_y()) * block.h;
    if (snake.is_alive())
    {
        SDL_SetRenderDrawColor(_sdl_renderer.get(), head_alive_r, head_alive_g,
                               head_alive_b, alpha_full);
    }
    else
    {
        SDL_SetRenderDrawColor(_sdl_renderer.get(), head_dead_r, 0x00, 0x00,
                               alpha_full);
    }
    SDL_RenderFillRect(_sdl_renderer.get(), &block);

    // Update Screen
    SDL_RenderPresent(_sdl_renderer.get());
}

void Renderer::update_window_title(int score, int fps)
{
    std::string title{"Snake Score: " + std::to_string(score) +
                      " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(_sdl_window.get(), title.c_str());
}
