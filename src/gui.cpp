#include "gui.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ncurses.h"

auto menu_choice(WINDOW *menu_window, std::vector<std::string> const &choices) -> int
{
    keypad(menu_window, true);
    curs_set(0);
    int choice{0};
    int highlight = 0;
    while (true)
    {
        for (int i = 0; i < choices.size(); ++i)
        {
            if (i == highlight)
            {
                wattron(menu_window, A_REVERSE);
            }
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
            mvwprintw(menu_window, i + 1, 1, choices[i].c_str());
            wattroff(menu_window, A_REVERSE);
        }
        choice = wgetch(menu_window);
        switch (choice)
        {
        case KEY_UP:
            highlight = std::max(--highlight, 0);
            break;
        case KEY_DOWN:
            highlight = std::min(++highlight, static_cast<int>(choices.size() - 1));
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
    return std::make_pair(y_max, x_max);
}

auto make_title(std::string const &title, int color_pair) -> WINDOW *
{
    auto [y_max, x_max] = get_max(stdscr);
    WINDOW *window      = newwin(3, x_max, 0, 0);
    wbkgd(window, COLOR_PAIR(color_pair));
    box(window, 0, 0);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    mvwprintw(window, 1, 1, title.c_str());
    refresh();
    wrefresh(window);
    return window;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto make_menu(int height, int color_pair) -> WINDOW *
{
    auto [y_max, x_max] = get_max(stdscr);
    WINDOW *window      = newwin(height + 2, x_max, 3, 0);
    wbkgd(window, COLOR_PAIR(color_pair));
    box(window, 0, 0);
    refresh();
    wrefresh(window);
    return window;
}

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
    init_pair(0, COLOR_BLUE, COLOR_WHITE);
    init_pair(1, COLOR_RED, COLOR_YELLOW);
    init_pair(2, COLOR_MAGENTA, COLOR_CYAN);
    auto menu      = _factory->create_main_menu();
    int color_pair = 0;
    while (menu)
    {
        make_title(menu->title(), color_pair);
        WINDOW *menu_window =
            make_menu(static_cast<int>(menu->options().size()), color_pair);
        refresh();
        int choice = menu_choice(menu_window, menu->options());
        menu       = menu->next(choice);
        ++color_pair %= 3;
    }
}
