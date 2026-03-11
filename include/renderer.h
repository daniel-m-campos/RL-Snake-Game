#pragma once

#include "food.h"
#include "snake.h"
#include <cstddef>

struct SDL_Window;   // IWYU pragma: keep
struct SDL_Renderer; // IWYU pragma: keep

class Renderer
{
  public:
    // NOLINT(bugprone-easily-swappable-parameters)
    Renderer(std::size_t screen_width, std::size_t screen_height,
             std::size_t grid_width, std::size_t grid_height);
    Renderer(Renderer const &)                     = delete;
    auto operator=(Renderer const &) -> Renderer & = delete;
    Renderer(Renderer &&)                          = default;
    auto operator=(Renderer &&) -> Renderer &      = default;
    ~Renderer();

    void render(snake::Snake const &snake, Food const &food);
    void update_window_title(int score, int fps);

  private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    std::size_t screen_width;
    std::size_t screen_height;
    std::size_t grid_width;
    std::size_t grid_height;
};
