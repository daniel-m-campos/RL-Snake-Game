#include "menu.h"

MainMenu::MainMenu(MenuFactory* factory, std::function<void(void)> snake_game)
    : _factory{factory}, _snake_game{std::move(snake_game)} {}

std::unique_ptr<Menu> MainMenu::Next(int choice) {
  switch (choice) {
    case 0:
      _snake_game();
      return _factory->CreateMainMenu();
    case 1:
      return _factory->CreateWatchMenu();
    case 2:
      return _factory->CreateTrainMenu();
    default:
      return std::unique_ptr<Menu>(nullptr);
  }
}

const std::string& MainMenu::Title() { return _title; }

const std::vector<std::string>& MainMenu::Options() { return _options; }

WatchMenu::WatchMenu(MenuFactory* factory) : _factory{factory} {}

const std::string& WatchMenu::Title() { return _title; }

const std::vector<std::string>& WatchMenu::Options() { return _options; }
std::unique_ptr<Menu> WatchMenu::Next(int choice) {
  switch (choice) {
    case 0:
      return _factory->CreateSelectBotMenu();
      ;
    case 1:
      return _factory->CreateMainMenu();
    default:
      return std::unique_ptr<Menu>(nullptr);
  }
}

SelectBotMenu::SelectBotMenu(MenuFactory* factory,
                             std::vector<std::string> bots,
                             std::function<void(const std ::string&)> watch_bot)
    : _factory{factory},
      _bots{std::move(bots)},
      _watch_bot{std::move(watch_bot)} {}

const std::string& SelectBotMenu::Title() { return _title; }

const std::vector<std::string>& SelectBotMenu::Options() { return _bots; }

std::unique_ptr<Menu> SelectBotMenu::Next(int choice) {
  if (choice < _bots.size()) {
    _watch_bot(_bots[choice]);
    return _factory->CreateMainMenu();
  }
  return std::unique_ptr<Menu>(nullptr);
}

TrainMenu::TrainMenu(MenuFactory* factory) : _factory{factory} {}

const std::string& TrainMenu::Title() { return _title; }

const std::vector<std::string>& TrainMenu::Options() { return _options; }

std::unique_ptr<Menu> TrainMenu::Next(int choice) {
  switch (choice) {
    case 0:
      return _factory->CreateParametersMenu();
    case 1:
      return _factory->CreateMainMenu();
    default:
      return std::unique_ptr<Menu>(nullptr);
  }
}

ParametersMenu::ParametersMenu(
    MenuFactory* factory, std::vector<std::string> parameters,
    std::function<void(const std::string&)> train_bot)
    : _factory{factory},
      _parameters{parameters},
      _train_bot{std::move(train_bot)} {
  _parameters.emplace_back("Back to Main Menu");
}

const std::string& ParametersMenu::Title() { return _title; }

const std::vector<std::string>& ParametersMenu::Options() {
  return _parameters;
}

std::unique_ptr<Menu> ParametersMenu::Next(int choice) {
  if (choice < _parameters.size()) {
    _train_bot(_parameters[choice]);
    return _factory->CreateMainMenu();
  }
  return std::unique_ptr<Menu>(nullptr);
}
