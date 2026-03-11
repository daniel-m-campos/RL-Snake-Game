#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Menu
{
  public:
    Menu()                                            = default;
    Menu(Menu const &)                                = default;
    Menu(Menu &&)                                     = default;
    auto operator=(Menu const &) -> Menu &            = default;
    auto operator=(Menu &&) -> Menu &                 = default;
    virtual ~Menu()                                   = default;
    virtual std::string const &title()                = 0;
    virtual std::vector<std::string> const &options() = 0;
    virtual std::unique_ptr<Menu> next(int choice)    = 0;
};

class MenuFactory
{
  public:
    MenuFactory()                                          = default;
    MenuFactory(MenuFactory const &)                       = default;
    MenuFactory(MenuFactory &&)                            = default;
    auto operator=(MenuFactory const &) -> MenuFactory &   = default;
    auto operator=(MenuFactory &&) -> MenuFactory &        = default;
    virtual ~MenuFactory()                                 = default;
    virtual std::unique_ptr<Menu> create_main_menu()       = 0;
    virtual std::unique_ptr<Menu> create_watch_menu()      = 0;
    virtual std::unique_ptr<Menu> create_train_menu()      = 0;
    virtual std::unique_ptr<Menu> create_select_bot_menu() = 0;
    virtual std::unique_ptr<Menu> create_parameters_menu() = 0;
};

class MainMenu : public Menu
{
  public:
    explicit MainMenu(MenuFactory *factory, std::function<void(void)> snake_game);
    std::string const &title() override;
    std::vector<std::string> const &options() override;
    std::unique_ptr<Menu> next(int choice) override;

  private:
    MenuFactory *_factory;
    std::string _title{"Main Menu"};
    std::vector<std::string> _options{"Play Snake!", "Watch Bot Play!!",
                                      "Train Bot to Play!!!"};
    std::function<void(void)> _snake_game;
};

class WatchMenu : public Menu
{
  public:
    explicit WatchMenu(MenuFactory *factory);
    std::string const &title() override;
    std::vector<std::string> const &options() override;
    std::unique_ptr<Menu> next(int choice) override;

  private:
    MenuFactory *_factory;
    std::string _title{"Watch Menu"};
    std::vector<std::string> _options{"Select Bot", "Back to MainMenu"};
};

class SelectBotMenu : public Menu
{
  public:
    SelectBotMenu(MenuFactory *factory, std::vector<std::string> bots,
                  std::function<void(std ::string const &)> watch_bot);
    auto title() -> std::string const & override;
    auto options() -> std::vector<std::string> const & override;
    auto next(int choice) -> std::unique_ptr<Menu> override;

  private:
    MenuFactory *_factory;
    std::string _title{"Select Bot Menu"};
    std::vector<std::string> _bots;
    std::function<void(std ::string const &)> _watch_bot;
};

class TrainMenu : public Menu
{
  public:
    explicit TrainMenu(MenuFactory *factory);
    auto title() -> std::string const & override;
    auto options() -> std::vector<std::string> const & override;
    auto next(int choice) -> std::unique_ptr<Menu> override;

  private:
    MenuFactory *_factory;
    std::string _title{"Train Menu"};
    std::vector<std::string> _options{"Enter Parameters", "Back to MainMenu"};
};

class ParametersMenu : public Menu
{
  public:
    ParametersMenu(MenuFactory *factory, std::vector<std ::string> parameters,
                   std::function<void(std ::string const &)> train_bot);
    std::string const &title() override;
    std::vector<std::string> const &options() override;
    std::unique_ptr<Menu> next(int choice) override;

  private:
    MenuFactory *_factory;
    std::string _title{"Parameters Menu"};
    std::vector<std::string> _parameters;
    std::function<void(std ::string const &)> _train_bot;
};
