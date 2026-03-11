#pragma once

#include <memory>
#include <utility>

#include "SDL.h"
#include "food.h"
#include "snake.h"
#include <cstddef>

namespace renderer_detail
{

// RAII guard for SDL_Init / SDL_Quit (supports move for Renderer move semantics)
struct SdlGuard
{
    bool _owns{false};

    explicit SdlGuard(Uint32 flags) : _owns{SDL_Init(flags) >= 0} {}
    SdlGuard(SdlGuard const &)                     = delete;
    auto operator=(SdlGuard const &) -> SdlGuard & = delete;
    SdlGuard(SdlGuard &&other) noexcept : _owns{std::exchange(other._owns, false)} {}
    auto operator=(SdlGuard &&other) noexcept -> SdlGuard &
    {
        if (this != &other)
        {
            if (_owns)
                SDL_Quit();
            _owns = std::exchange(other._owns, false);
        }
        return *this;
    }
    ~SdlGuard()
    {
        if (_owns)
            SDL_Quit();
    }
};

struct SdlWindowDeleter
{
    void operator()(SDL_Window *w) const
    {
        SDL_DestroyWindow(w);
    }
};

struct SdlRendererDeleter
{
    void operator()(SDL_Renderer *r) const
    {
        SDL_DestroyRenderer(r);
    }
};

using SdlWindowPtr   = std::unique_ptr<SDL_Window, SdlWindowDeleter>;
using SdlRendererPtr = std::unique_ptr<SDL_Renderer, SdlRendererDeleter>;

} // namespace renderer_detail

class Renderer
{
  public:
    Renderer(std::size_t screen_width, std::size_t screen_height,
             std::size_t grid_width, std::size_t grid_height);
    Renderer(Renderer const &)                     = delete;
    auto operator=(Renderer const &) -> Renderer & = delete;
    Renderer(Renderer &&)                          = default;
    auto operator=(Renderer &&) -> Renderer &      = default;
    ~Renderer()                                    = default;

    void render(snake::Snake const &snake, Food const &food);
    void update_window_title(int score, int fps);

  private:
    // _sdl_guard must be declared first so SDL is initialised before the window
    // and renderer, and destroyed after them (members are destroyed in reverse order).
    renderer_detail::SdlGuard _sdl_guard{SDL_INIT_VIDEO};
    renderer_detail::SdlWindowPtr _sdl_window;
    renderer_detail::SdlRendererPtr _sdl_renderer;

    std::size_t screen_width;
    std::size_t screen_height;
    std::size_t grid_width;
    std::size_t grid_height;
};
