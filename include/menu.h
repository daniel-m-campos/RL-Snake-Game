#ifndef RLSNAKEGAME_MENU_H
#define RLSNAKEGAME_MENU_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Menu {
 public:
  virtual ~Menu() = default;
  virtual const std::string& Title() = 0;
  virtual const std::vector<std::string>& Options() = 0;
  virtual std::unique_ptr<Menu> Next(int choice) = 0;
};

class MenuFactory {
 public:
  virtual ~MenuFactory() = default;
  virtual std::unique_ptr<Menu> CreateMainMenu() = 0;
  virtual std::unique_ptr<Menu> CreateWatchMenu() = 0;
  virtual std::unique_ptr<Menu> CreateTrainMenu() = 0;
  virtual std::unique_ptr<Menu> CreateSelectBotMenu() = 0;
  virtual std::unique_ptr<Menu> CreateParametersMenu() = 0;
};

class MainMenu : public Menu {
 public:
  explicit MainMenu(MenuFactory* factory, std::function<void(void)> snake_game);
  const std::string& Title() override;
  const std::vector<std::string>& Options() override;
  std::unique_ptr<Menu> Next(int choice) override;

 private:
  MenuFactory* _factory;
  const std::string _title{"Main Menu"};
  const std::vector<std::string> _options{"Play Snake!", "Watch Bot Play!!",
                                          "Train Bot to Play!!!"};
  std::function<void(void)> _snake_game;
};

class WatchMenu : public Menu {
 public:
  explicit WatchMenu(MenuFactory* factory);
  const std::string& Title() override;
  const std::vector<std::string>& Options() override;
  std::unique_ptr<Menu> Next(int choice) override;

 private:
  MenuFactory* _factory;
  const std::string _title{"Watch Menu"};
  const std::vector<std::string> _options{"Select Bot", "Back to MainMenu"};
};

class SelectBotMenu : public Menu {
 public:
  SelectBotMenu(MenuFactory* factory, std::vector<std::string> bots,
                std::function<void(const std ::string&)> watch_bot);
  const std::string& Title() override;
  const std::vector<std::string>& Options() override;
  std::unique_ptr<Menu> Next(int choice) override;

 private:
  MenuFactory* _factory;
  const std::string _title{"Select Bot Menu"};
  std::vector<std::string> _bots;
  std::function<void(const std ::string&)> _watch_bot;
};

class TrainMenu : public Menu {
 public:
  explicit TrainMenu(MenuFactory* factory);
  const std::string& Title() override;
  const std::vector<std::string>& Options() override;
  std::unique_ptr<Menu> Next(int choice) override;

 private:
  MenuFactory* _factory;
  const std::string _title{"Train Menu"};
  const std::vector<std::string> _options{"Enter Parameters",
                                          "Back to MainMenu"};
};

class ParametersMenu : public Menu {
 public:
  ParametersMenu(MenuFactory* factory, std::vector<std ::string> parameters,
                 std::function<void(const std ::string&)> train_bot);
  const std::string& Title() override;
  const std::vector<std::string>& Options() override;
  std::unique_ptr<Menu> Next(int choice) override;

 private:
  MenuFactory* _factory;
  const std::string _title{"Parameters Menu"};
  std::vector<std::string> _parameters;
  std::function<void(const std ::string&)> _train_bot;
};

#endif  // RLSNAKEGAME_MENU_H
