#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "menu.h"
#include "ncurses.h"

// RAII wrapper for ncurses WINDOW*
struct NcursesWinDeleter
{
    void operator()(WINDOW *w) const
    {
        delwin(w);
    }
};
using NcursesWinPtr = std::unique_ptr<WINDOW, NcursesWinDeleter>;

[[nodiscard]] auto menu_choice(WINDOW &menu_window,
                               std::vector<std::string> const &choices) -> int;

[[nodiscard]] auto get_max(WINDOW *window) -> std::pair<int, int>;

[[nodiscard]] auto make_title(std::string const &title, int color_pair)
    -> NcursesWinPtr;

[[nodiscard]] auto make_menu(int height, int color_pair) -> NcursesWinPtr;

class GUI
{
  public:
    explicit GUI(std::unique_ptr<MenuFactory> factory);
    GUI(const GUI &)                     = delete;
    auto operator=(const GUI &) -> GUI & = delete;
    GUI(GUI &&)                          = default;
    auto operator=(GUI &&) -> GUI &      = default;
    ~GUI();
    void show();

  private:
    std::unique_ptr<MenuFactory> _factory;

    static void init_ncurses();
};
