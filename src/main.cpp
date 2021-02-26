#include "game_menu_factory.h"
#include "gui.h"

int main() {
  GUI gui{std::make_unique<GameMenuFactory>()};
  gui.Show();
}