#include "gui.h"

#include <iostream>
#include <memory>

#include "ncurses.h"

int MenuChoice(WINDOW* menu_window, const std::vector<std::string>& choices) {
  keypad(menu_window, true);
  curs_set(0);
  int choice;
  int highlight = 0;
  while (true) {
    for (int i = 0; i < choices.size(); ++i) {
      if (i == highlight) {
        wattron(menu_window, A_REVERSE);
      }
      mvwprintw(menu_window, i + 1, 1, choices[i].c_str());
      wattroff(menu_window, A_REVERSE);
    }
    choice = wgetch(menu_window);
    switch (choice) {
      case KEY_UP:
        highlight = std::max(--highlight, 0);
        break;
      case KEY_DOWN:
        highlight = std::min(++highlight, static_cast<int>(choices.size() - 1));
        break;
      default:
        break;
    }
    if (choice == 10 | choice == KEY_ENTER) break;
  }
  clear();
  curs_set(1);
  return highlight;
}

std::pair<int, int> GetMax(WINDOW* window) {
  int y_max, x_max;
  getmaxyx(window, y_max, x_max);
  return std::make_pair(y_max, x_max);
}

WINDOW* MakeTitle(const std::string& title, int color_pair) {
  auto [y_max, x_max] = GetMax(stdscr);
  WINDOW* window = newwin(3, x_max, 0, 0);
  wbkgd(window, COLOR_PAIR(color_pair));
  box(window, 0, 0);
  mvwprintw(window, 1, 1, title.c_str());
  refresh();
  wrefresh(window);
  return window;
}

WINDOW* MakeMenu(int height, int color_pair) {
  auto [y_max, x_max] = GetMax(stdscr);
  WINDOW* window = newwin(height + 2, x_max, 3, 0);
  wbkgd(window, COLOR_PAIR(color_pair));
  box(window, 0, 0);
  refresh();
  wrefresh(window);
  return window;
}

void GUI::InitNCurses() {
  initscr();
  noecho();
  cbreak();
  start_color();
}

GUI::GUI(std::unique_ptr<MenuFactory> factory) : _factory{std::move(factory)} {
  InitNCurses();
}

GUI::~GUI() { endwin(); }

void GUI::Show() {
  init_pair(0, COLOR_BLUE, COLOR_WHITE);
  init_pair(1, COLOR_RED, COLOR_YELLOW);
  init_pair(2, COLOR_MAGENTA, COLOR_CYAN);
  auto menu = _factory->CreateMainMenu();
  int color_pair = 0;
  while (menu) {
    MakeTitle(menu->Title(), color_pair);
    WINDOW* menu_window = MakeMenu(menu->Options().size(), color_pair);
    refresh();
    int choice = MenuChoice(menu_window, menu->Options());
    menu = menu->Next(choice);
    ++color_pair %= 3;
  }
}
