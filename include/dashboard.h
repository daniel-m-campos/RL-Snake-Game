#pragma once

#include "training_config.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "ncurses.h"

// RAII wrapper for ncurses WINDOW*
struct NcursesWinDeleter
{
    void operator()(WINDOW *w) const
    {
        if (w != nullptr)
        {
            delwin(w);
        }
    }
};
using NcursesWinPtr = std::unique_ptr<WINDOW, NcursesWinDeleter>;

class Dashboard
{
  public:
    Dashboard();

    void rebuild_layout();

    void render_nav(std::vector<std::string> const &items, int highlight);
    void render_welcome();
    void render_params(TrainingConfig const &config, int highlight, int editing_index,
                       std::string const &edit_buffer);
    void render_progress(TrainingProgress const &progress,
                         std::chrono::steady_clock::time_point start_time);
    void render_status(std::string const &message);

    [[nodiscard]] auto nav_choice(std::vector<std::string> const &items) -> int;
    [[nodiscard]] auto poll_input(int timeout_ms) -> int;
    [[nodiscard]] auto content_input() -> int;

  private:
    NcursesWinPtr _nav_panel;
    NcursesWinPtr _content_panel;
    NcursesWinPtr _status_bar;
    int _nav_width{20};

    void init_colors();
    void create_panels();
    void draw_progress_bar(WINDOW &win, int row, int col, int width, double fraction);
};
