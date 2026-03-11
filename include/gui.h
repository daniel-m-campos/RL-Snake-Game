#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "dashboard.h"
#include "menu.h"
#include "ncurses.h"

[[nodiscard]] auto menu_choice(WINDOW &menu_window,
                               std::vector<std::string> const &choices) -> int;

[[nodiscard]] auto get_max(WINDOW *window) -> std::pair<int, int>;

[[nodiscard]] auto make_title(std::string const &title, int color_pair)
    -> NcursesWinPtr;

[[nodiscard]] auto make_menu(int height, int color_pair) -> NcursesWinPtr;

class TrainingSetupMenu;
class TrainingActiveMenu;

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
    std::unique_ptr<Dashboard> _dashboard;

    static void init_ncurses();
    auto handle_param_editing(TrainingSetupMenu &setup_menu) -> bool;
    void handle_training_progress(TrainingActiveMenu &active_menu);
};
