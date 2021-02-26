#ifndef RLSNAKEGAME_GUI_H
#define RLSNAKEGAME_GUI_H

#include <string>
#include <vector>

#include "menu.h"
#include "ncurses.h"

int MenuChoice(WINDOW* menu_window, const std::vector<std::string>& choices);

std::pair<int, int> GetMax(WINDOW* window);

WINDOW* MakeTitle(const std::string& title, int color_pair);

WINDOW* MakeMenu(int height, int color_pair);

class GUI {
 public:
  explicit GUI(std::unique_ptr<MenuFactory> factory);
  ~GUI();
  void Show();

 private:
  std::unique_ptr<MenuFactory> _factory;

  static void InitNCurses();
};

#endif  // RLSNAKEGAME_GUI_H
