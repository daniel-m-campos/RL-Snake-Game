#include "gui.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "dashboard.h"
#include "menu.h"
#include "training_config.h"

#include "ncurses.h"

auto menu_choice(WINDOW &menu_window, std::vector<std::string> const &choices) -> int
{
    keypad(&menu_window, true);
    curs_set(0);
    int choice{0};
    int highlight = 0;
    while (true)
    {
        for (std::size_t i = 0; i < choices.size(); ++i)
        {
            if (static_cast<int>(i) == highlight)
            {
                wattron(&menu_window, A_REVERSE);
            }
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
            mvwprintw(&menu_window, static_cast<int>(i) + 1, 1, "%s",
                      choices[i].c_str());
            wattroff(&menu_window, A_REVERSE);
        }
        choice = wgetch(&menu_window);
        switch (choice)
        {
        case KEY_UP:
            --highlight;
            highlight = std::max(highlight, 0);
            break;
        case KEY_DOWN:
            ++highlight;
            highlight = std::min(highlight, static_cast<int>(choices.size() - 1));
            break;
        default:
            break;
        }
        int constexpr enter_key{10};
        if (choice == enter_key || choice == KEY_ENTER)
        {
            break;
        }
    }
    clear();
    curs_set(1);
    return highlight;
}

std::pair<int, int> get_max(WINDOW *window)
{
    int y_max{0};
    int x_max{0};
    getmaxyx(window, y_max, x_max);
    return {y_max, x_max};
}

auto make_title(std::string const &title, int color_pair) -> NcursesWinPtr
{
    auto [y_max, x_max] = get_max(stdscr);
    NcursesWinPtr window{newwin(3, x_max, 0, 0)};
    wbkgd(window.get(), COLOR_PAIR(color_pair));
    box(window.get(), 0, 0);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(window.get(), 1, 1, "%s", title.c_str());
    refresh();
    wrefresh(window.get());
    return window;
}

auto make_menu(int height, int color_pair) -> NcursesWinPtr
{
    auto [y_max, x_max] = get_max(stdscr);
    NcursesWinPtr window{newwin(height + 2, x_max, 3, 0)};
    wbkgd(window.get(), COLOR_PAIR(color_pair));
    box(window.get(), 0, 0);
    refresh();
    wrefresh(window.get());
    return window;
}

namespace
{
auto get_param_value_string(TrainingConfig const &config, int index) -> std::string
{
    switch (index)
    {
    case 0:
        return std::to_string(config.grid_width);
    case 1:
        return std::to_string(config.grid_height);
    case 2:
        return std::to_string(config.num_episodes);
    case 3:
        return std::to_string(config.max_steps);
    case 4:
    {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.4f", config.epsilon);
        return buf;
    }
    case 5:
    {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.4f", config.discount_factor);
        return buf;
    }
    case 6:
    {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.4f", config.step_size);
        return buf;
    }
    case 7:
        return std::to_string(config.num_threads);
    case 8:
        return std::to_string(config.checkpoint_interval);
    default:
        return {};
    }
}

void apply_param_edit(TrainingConfig &config, int index, std::string const &value)
{
    try
    {
        switch (index)
        {
        case 0:
            config.grid_width = std::stoull(value);
            break;
        case 1:
            config.grid_height = std::stoull(value);
            break;
        case 2:
            config.num_episodes = std::stoll(value);
            break;
        case 3:
            config.max_steps = std::stoll(value);
            break;
        case 4:
            config.epsilon = std::stod(value);
            break;
        case 5:
            config.discount_factor = std::stod(value);
            break;
        case 6:
            config.step_size = std::stod(value);
            break;
        case 7:
            config.num_threads = std::stoi(value);
            break;
        case 8:
            config.checkpoint_interval = std::stoll(value);
            break;
        default:
            break;
        }
    }
    catch (...)
    {
        // Invalid input; keep existing value
    }
}
} // namespace

void GUI::init_ncurses()
{
    initscr();
    noecho();
    cbreak();
    start_color();
}

GUI::GUI(std::unique_ptr<MenuFactory> factory) : _factory{std::move(factory)}
{
    init_ncurses();
}

GUI::~GUI()
{
    endwin();
}

void GUI::show()
{
    _dashboard = std::make_unique<Dashboard>();
    auto menu  = _factory->create_main_menu();

    while (menu)
    {
        _dashboard->render_status(menu->title());

        auto *setup_menu = dynamic_cast<TrainingSetupMenu *>(menu.get());
        if (setup_menu != nullptr)
        {
            if (handle_param_editing(*setup_menu))
            {
                auto progress        = std::make_shared<TrainingProgress>();
                auto const &callback = setup_menu->get_train_callback();
                auto const &config   = setup_menu->get_config();

                std::jthread training_thread([&callback, &config, progress]()
                                             { callback(config, progress); });

                TrainingActiveMenu active(*_factory, progress);
                handle_training_progress(active);
                // jthread joins on destruction
            }
            menu = _factory->create_main_menu();
            continue;
        }

        _dashboard->render_welcome();
        int choice = _dashboard->nav_choice(menu->options());
        menu       = menu->next(choice);

        // After play() or other SDL actions, ncurses screen may be corrupted.
        // Force a full redraw by clearing and rebuilding the layout.
        clearok(curscr, TRUE);
        _dashboard->rebuild_layout();
    }
}

auto GUI::handle_param_editing(TrainingSetupMenu &setup_menu) -> bool
{
    auto config         = setup_menu.get_config();
    int param_highlight = 0;
    int editing_index   = -1;
    std::string edit_buffer;
    int constexpr num_params = 9;
    int constexpr enter_key  = 10;

    _dashboard->render_params(config, param_highlight, editing_index, edit_buffer);
    _dashboard->render_nav(setup_menu.options(), -1);
    _dashboard->render_status("Up/Down: navigate | Enter: edit | Tab: nav menu");

    while (true)
    {
        int ch = _dashboard->poll_input(100);
        if (ch == ERR)
        {
            continue;
        }

        if (ch == '\t')
        {
            int nav_choice = _dashboard->nav_choice(setup_menu.options());
            if (nav_choice == 0)
            {
                setup_menu.set_config(config);
                return true;
            }
            if (nav_choice == 1)
            {
                return false;
            }
            _dashboard->render_params(config, param_highlight, editing_index,
                                      edit_buffer);
            _dashboard->render_status(
                "Up/Down: navigate | Enter: edit | Tab: nav menu");
            continue;
        }

        if (editing_index >= 0)
        {
            if (ch == enter_key || ch == KEY_ENTER)
            {
                apply_param_edit(config, editing_index, edit_buffer);
                editing_index = -1;
                edit_buffer.clear();
            }
            else if (ch == KEY_BACKSPACE || ch == 127)
            {
                if (!edit_buffer.empty())
                {
                    edit_buffer.pop_back();
                }
            }
            else if (ch == 27)
            {
                editing_index = -1;
                edit_buffer.clear();
            }
            else if (std::isprint(ch) != 0)
            {
                edit_buffer += static_cast<char>(ch);
            }
        }
        else
        {
            if (ch == KEY_UP && param_highlight > 0)
            {
                --param_highlight;
            }
            else if (ch == KEY_DOWN && param_highlight < num_params - 1)
            {
                ++param_highlight;
            }
            else if (ch == enter_key || ch == KEY_ENTER)
            {
                editing_index = param_highlight;
                edit_buffer   = get_param_value_string(config, param_highlight);
            }
            else if (ch == 'q')
            {
                return false;
            }
        }

        _dashboard->render_params(config, param_highlight, editing_index, edit_buffer);
    }
}

void GUI::handle_training_progress(TrainingActiveMenu &active_menu)
{
    auto progress   = active_menu.get_progress();
    auto start_time = std::chrono::steady_clock::now();

    _dashboard->render_nav(active_menu.options(), -1);
    _dashboard->render_status("Press 'c' to cancel training");

    while (!progress->training_complete.load())
    {
        _dashboard->render_progress(*progress, start_time);

        int ch = _dashboard->poll_input(200);
        if (ch == 'c' || ch == 'q')
        {
            progress->cancel_requested.store(true);
            _dashboard->render_status("Cancelling training...");
        }
        if (ch == KEY_RESIZE)
        {
            _dashboard->rebuild_layout();
        }
    }

    _dashboard->render_progress(*progress, start_time);
    _dashboard->render_status("Training complete! Press any key to continue...");
    static_cast<void>(_dashboard->poll_input(-1));
}
