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

TrainingSetupMenu::TrainingSetupMenu(MenuFactory &factory, TrainingConfig config,
                                     TrainCallback callback)
    : _factory{factory}, _config{std::move(config)},
      _train_callback{std::move(callback)}
{
}

auto TrainingSetupMenu::title() -> std::string const &
{
    return _title;
}

auto TrainingSetupMenu::options() -> std::vector<std::string> const &
{
    return _options;
}

auto TrainingSetupMenu::next(int choice) -> std::unique_ptr<Menu>
{
    switch (choice)
    {
    case 0:
        return nullptr;
    case 1:
        return _factory.create_main_menu();
    default:
        return nullptr;
    }
}

auto TrainingSetupMenu::get_config() const -> TrainingConfig const &
{
    return _config;
}

void TrainingSetupMenu::set_config(TrainingConfig const &config)
{
    _config = config;
}

auto TrainingSetupMenu::get_train_callback() const -> TrainCallback const &
{
    return _train_callback;
}

TrainingActiveMenu::TrainingActiveMenu(MenuFactory &factory,
                                       std::shared_ptr<TrainingProgress> progress)
    : _factory{factory}, _progress{std::move(progress)}
{
}

auto TrainingActiveMenu::title() -> std::string const &
{
    return _title;
}

auto TrainingActiveMenu::options() -> std::vector<std::string> const &
{
    return _options;
}

auto TrainingActiveMenu::next(int choice) -> std::unique_ptr<Menu>
{
    if (choice == 0 && _progress)
    {
        _progress->cancel_requested.store(true);
    }
    return _factory.create_main_menu();
}

auto TrainingActiveMenu::get_progress() const -> std::shared_ptr<TrainingProgress>
{
    return _progress;
}
