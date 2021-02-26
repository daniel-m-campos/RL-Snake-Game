#ifndef RLSNAKEGAME_GAME_MENU_FACTORY_H
#define RLSNAKEGAME_GAME_MENU_FACTORY_H

#include "gui.h"

class GameMenuFactory : public MenuFactory {
 public:
  std::unique_ptr<Menu> CreateMainMenu() override;
  std::unique_ptr<Menu> CreateWatchMenu() override;
  std::unique_ptr<Menu> CreateTrainMenu() override;
  std::unique_ptr<Menu> CreateSelectBotMenu() override;
  std::unique_ptr<Menu> CreateParametersMenu() override;

 private:
  static std::pair<size_t, size_t> GetGridSize(const std::string& filename);
};

#endif  // RLSNAKEGAME_GAME_MENU_FACTORY_H
