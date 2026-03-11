#include "menu.h"
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

MainMenu::MainMenu(MenuFactory &factory, std::function<void(void)> snake_game)
    : _factory{factory}, _snake_game{std::move(snake_game)}
{
}

std::unique_ptr<Menu> MainMenu::next(int choice)
{
    switch (choice)
    {
    case 0:
        _snake_game();
        return _factory.create_main_menu();
    case 1:
        return _factory.create_watch_menu();
    case 2:
        return _factory.create_train_menu();
    default:
        return {nullptr};
    }
}

std::string const &MainMenu::title()
{
    return _title;
}

std::vector<std::string> const &MainMenu::options()
{
    return _options;
}

WatchMenu::WatchMenu(MenuFactory &factory) : _factory{factory} {}

std::string const &WatchMenu::title()
{
    return _title;
}

std::vector<std::string> const &WatchMenu::options()
{
    return _options;
}
std::unique_ptr<Menu> WatchMenu::next(int choice)
{
    switch (choice)
    {
    case 0:
        return _factory.create_select_bot_menu();
    case 1:
        return _factory.create_main_menu();
    default:
        return {nullptr};
    }
}

SelectBotMenu::SelectBotMenu(MenuFactory &factory, std::vector<std::string> bots,
                             std::function<void(std ::string const &)> watch_bot)
    : _factory{factory}, _bots{std::move(bots)}, _watch_bot{std::move(watch_bot)}
{
}

std::string const &SelectBotMenu::title()
{
    return _title;
}

std::vector<std::string> const &SelectBotMenu::options()
{
    return _bots;
}

std::unique_ptr<Menu> SelectBotMenu::next(int choice)
{
    if (choice < static_cast<int>(_bots.size()))
    {
        _watch_bot(_bots[choice]);
        return _factory.create_main_menu();
    }
    return {nullptr};
}

TrainMenu::TrainMenu(MenuFactory &factory) : _factory{factory} {}

std::string const &TrainMenu::title()
{
    return _title;
}

std::vector<std::string> const &TrainMenu::options()
{
    return _options;
}

auto TrainMenu::next(int choice) -> std::unique_ptr<Menu>
{
    switch (choice)
    {
    case 0:
        return _factory.create_parameters_menu();
    case 1:
        return _factory.create_main_menu();
    default:
        return {nullptr};
    }
}

ParametersMenu::ParametersMenu(MenuFactory &factory,
                               std::vector<std::string> parameters,
                               std::function<void(std::string const &)> train_bot)
    : _factory{factory}, _parameters{std::move(parameters)},
      _train_bot{std::move(train_bot)}
{
    _parameters.emplace_back("Back to Main Menu");
}

std::string const &ParametersMenu::title()
{
    return _title;
}

std::vector<std::string> const &ParametersMenu::options()
{
    return _parameters;
}

std::unique_ptr<Menu> ParametersMenu::next(int choice)
{
    if (choice < static_cast<int>(_parameters.size()) - 1)
    {
        _train_bot(_parameters[choice]);
    }
    if (choice < static_cast<int>(_parameters.size()))
    {
        return _factory.create_main_menu();
    }
    return {nullptr};
}
