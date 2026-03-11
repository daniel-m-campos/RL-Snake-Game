#include "dashboard.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

#include "ncurses.h"

namespace
{
auto format_double(double val) -> std::string
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%.4f", val);
    return buf;
}
} // namespace

Dashboard::Dashboard()
{
    init_colors();
    create_panels();
}

void Dashboard::init_colors()
{
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_GREEN);
}

void Dashboard::create_panels()
{
    int rows = 0;
    int cols = 0;
    getmaxyx(stdscr, rows, cols);

    _nav_width = std::max(20, cols * 3 / 10);

    _nav_panel.reset(newwin(rows - 1, _nav_width, 0, 0));
    keypad(_nav_panel.get(), TRUE);
    wbkgd(_nav_panel.get(), COLOR_PAIR(1));
    box(_nav_panel.get(), 0, 0);
    wrefresh(_nav_panel.get());

    _content_panel.reset(newwin(rows - 1, cols - _nav_width, 0, _nav_width));
    wbkgd(_content_panel.get(), COLOR_PAIR(2));
    box(_content_panel.get(), 0, 0);
    wrefresh(_content_panel.get());

    _status_bar.reset(newwin(1, cols, rows - 1, 0));
    wbkgd(_status_bar.get(), COLOR_PAIR(3));
    wrefresh(_status_bar.get());
}

void Dashboard::rebuild_layout()
{
    _nav_panel.reset();
    _content_panel.reset();
    _status_bar.reset();
    clear();
    create_panels();
}

void Dashboard::render_nav(std::vector<std::string> const &items, int highlight)
{
    werase(_nav_panel.get());
    box(_nav_panel.get(), 0, 0);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_nav_panel.get(), 1, 2, "Navigation");

    for (std::size_t i = 0; i < items.size(); ++i)
    {
        if (static_cast<int>(i) == highlight)
        {
            wattron(_nav_panel.get(), A_REVERSE);
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_nav_panel.get(), static_cast<int>(i) + 3, 2, "%s", items[i].c_str());
        wattroff(_nav_panel.get(), A_REVERSE);
    }
    wrefresh(_nav_panel.get());
}

void Dashboard::render_welcome()
{
    werase(_content_panel.get());
    box(_content_panel.get(), 0, 0);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 2, 2, "Welcome to RL Snake Game!");
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 4, 2, "Use the navigation panel to:");
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 5, 2, "  - Play Snake");
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 6, 2, "  - Watch a trained bot play");
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 7, 2, "  - Train a new bot");
    wrefresh(_content_panel.get());
}

void Dashboard::render_params(TrainingConfig const &config, int highlight,
                              int editing_index, std::string const &edit_buffer)
{
    werase(_content_panel.get());
    box(_content_panel.get(), 0, 0);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 1, 2, "Training Parameters");

    std::vector<std::pair<std::string, std::string>> const params = {
        {"Grid Width", std::to_string(config.grid_width)},
        {"Grid Height", std::to_string(config.grid_height)},
        {"Episodes", std::to_string(config.num_episodes)},
        {"Max Steps", std::to_string(config.max_steps)},
        {"Epsilon", format_double(config.epsilon)},
        {"Discount Factor", format_double(config.discount_factor)},
        {"Step Size", format_double(config.step_size)},
        {"Threads", std::to_string(config.num_threads)},
        {"Checkpoint Int.", std::to_string(config.checkpoint_interval)},
    };

    for (std::size_t i = 0; i < params.size(); ++i)
    {
        int row = static_cast<int>(i) + 3;
        if (static_cast<int>(i) == highlight)
        {
            wattron(_content_panel.get(), A_REVERSE);
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_content_panel.get(), row, 2, "%-20s", params[i].first.c_str());

        if (static_cast<int>(i) == editing_index)
        {
            std::string display = edit_buffer + "_";
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
            mvwprintw(_content_panel.get(), row, 22, "%s", display.c_str());
        }
        else
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
            mvwprintw(_content_panel.get(), row, 22, "%s", params[i].second.c_str());
        }
        wattroff(_content_panel.get(), A_REVERSE);
    }
    wrefresh(_content_panel.get());
}

void Dashboard::render_progress(TrainingProgress const &progress,
                                std::chrono::steady_clock::time_point start_time)
{
    werase(_content_panel.get());
    box(_content_panel.get(), 0, 0);

    int64_t completed = progress.episodes_completed.load();
    int64_t total     = progress.total_episodes.load();
    double fraction =
        (total > 0) ? static_cast<double>(completed) / static_cast<double>(total) : 0.0;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 2, 2, "Training Progress");

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 4, 2, "Episodes: %lld / %lld",
              static_cast<long long>(completed), static_cast<long long>(total));

    int constexpr bar_width = 40;
    draw_progress_bar(*_content_panel, 6, 2, bar_width, fraction);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 7, 2, "%.1f%%", fraction * 100.0);

    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
    int64_t elapsed_secs = elapsed.count();
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 9, 2, "Elapsed: %02lld:%02lld",
              static_cast<long long>(elapsed_secs / 60),
              static_cast<long long>(elapsed_secs % 60));

    if (completed > 0 && total > completed)
    {
        int64_t eta_secs = elapsed_secs * (total - completed) / completed;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_content_panel.get(), 10, 2, "ETA: ~%02lld:%02lld",
                  static_cast<long long>(eta_secs / 60),
                  static_cast<long long>(eta_secs % 60));
    }
    else
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_content_panel.get(), 10, 2, "ETA: ~--:--");
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 12, 2, "Q-table entries: %lld",
              static_cast<long long>(progress.qtable_size.load()));
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_content_panel.get(), 13, 2, "Checkpoints: %lld",
              static_cast<long long>(progress.checkpoints_completed.load()));

    if (progress.training_complete.load())
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_content_panel.get(), 15, 2, "Complete! Model saved.");
    }
    else if (progress.saving.load())
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_content_panel.get(), 15, 2, "Saving model to disk...");
    }
    else if (progress.merging.load())
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_content_panel.get(), 15, 2, "Merging Q-tables...");
    }
    else if (progress.cancel_requested.load())
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_content_panel.get(), 15, 2, "Cancelling...");
    }
    else
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        mvwprintw(_content_panel.get(), 15, 2, "Training...");
    }

    wrefresh(_content_panel.get());
}

void Dashboard::render_status(std::string const &message)
{
    werase(_status_bar.get());
    wbkgd(_status_bar.get(), COLOR_PAIR(3));
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(_status_bar.get(), 0, 1, "%s", message.c_str());
    wrefresh(_status_bar.get());
}

auto Dashboard::nav_choice(std::vector<std::string> const &items) -> int
{
    keypad(_nav_panel.get(), TRUE);
    curs_set(0);

    int highlight           = 0;
    int constexpr enter_key = 10;

    while (true)
    {
        render_nav(items, highlight);

        int ch = wgetch(_nav_panel.get());
        switch (ch)
        {
        case KEY_UP:
            if (highlight > 0)
            {
                --highlight;
            }
            break;
        case KEY_DOWN:
            if (highlight < static_cast<int>(items.size()) - 1)
            {
                ++highlight;
            }
            break;
        case enter_key:
        case KEY_ENTER:
            return highlight;
        default:
            break;
        }
    }
}

auto Dashboard::poll_input(int timeout_ms) -> int
{
    wtimeout(_nav_panel.get(), timeout_ms);
    int ch = wgetch(_nav_panel.get());
    wtimeout(_nav_panel.get(), -1);
    return ch;
}

auto Dashboard::content_input() -> int
{
    keypad(_content_panel.get(), TRUE);
    return wgetch(_content_panel.get());
}

void Dashboard::draw_progress_bar(WINDOW &win, int row, int col, int width,
                                  double fraction)
{
    int filled = static_cast<int>(fraction * width);
    mvwaddch(&win, row, col, '[');
    for (int i = 0; i < width; ++i)
    {
        if (i < filled)
        {
            wattron(&win, COLOR_PAIR(4));
            waddch(&win, '#');
            wattroff(&win, COLOR_PAIR(4));
        }
        else
        {
            waddch(&win, '-');
        }
    }
    waddch(&win, ']');
}
