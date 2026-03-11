#pragma once

#include "training_config.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Menu
{
  public:
    Menu()                                                                   = default;
    Menu(Menu const &)                                                       = default;
    Menu(Menu &&)                                                            = default;
    auto operator=(Menu const &) -> Menu &                                   = default;
    auto operator=(Menu &&) -> Menu &                                        = default;
    virtual ~Menu()                                                          = default;
    [[nodiscard]] virtual auto title() -> std::string const &                = 0;
    [[nodiscard]] virtual auto options() -> std::vector<std::string> const & = 0;
    [[nodiscard]] virtual auto next(int choice) -> std::unique_ptr<Menu>     = 0;
};

class MenuFactory
{
  public:
    MenuFactory()                                                      = default;
    MenuFactory(MenuFactory const &)                                   = default;
    MenuFactory(MenuFactory &&)                                        = default;
    auto operator=(MenuFactory const &) -> MenuFactory &               = default;
    auto operator=(MenuFactory &&) -> MenuFactory &                    = default;
    virtual ~MenuFactory()                                             = default;
    virtual auto create_main_menu() -> std::unique_ptr<Menu>           = 0;
    virtual auto create_watch_menu() -> std::unique_ptr<Menu>          = 0;
    virtual auto create_train_menu() -> std::unique_ptr<Menu>          = 0;
    virtual auto create_select_bot_menu() -> std::unique_ptr<Menu>     = 0;
    virtual auto create_parameters_menu() -> std::unique_ptr<Menu>     = 0;
    virtual auto create_training_setup_menu() -> std::unique_ptr<Menu> = 0;
    virtual auto create_training_active_menu(std::shared_ptr<TrainingProgress> progress)
        -> std::unique_ptr<Menu> = 0;
};

class MainMenu : public Menu
{
  public:
    explicit MainMenu(MenuFactory &factory, std::function<void(void)> snake_game);
    [[nodiscard]] auto title() -> std::string const & override;
    [[nodiscard]] auto options() -> std::vector<std::string> const & override;
    [[nodiscard]] auto next(int choice) -> std::unique_ptr<Menu> override;

  private:
    MenuFactory &_factory;
    std::string _title{"Main Menu"};
    std::vector<std::string> _options{"Play Snake!", "Watch Bot Play!!",
                                      "Train Bot to Play!!!"};
    std::function<void(void)> _snake_game;
};

class WatchMenu : public Menu
{
  public:
    explicit WatchMenu(MenuFactory &factory);
    [[nodiscard]] auto title() -> std::string const & override;
    [[nodiscard]] auto options() -> std::vector<std::string> const & override;
    [[nodiscard]] auto next(int choice) -> std::unique_ptr<Menu> override;

  private:
    MenuFactory &_factory;
    std::string _title{"Watch Menu"};
    std::vector<std::string> _options{"Select Bot", "Back to MainMenu"};
};

class SelectBotMenu : public Menu
{
  public:
    SelectBotMenu(MenuFactory &factory, std::vector<std::string> bots,
                  std::function<void(std ::string const &)> watch_bot);
    [[nodiscard]] auto title() -> std::string const & override;
    [[nodiscard]] auto options() -> std::vector<std::string> const & override;
    [[nodiscard]] auto next(int choice) -> std::unique_ptr<Menu> override;

  private:
    MenuFactory &_factory;
    std::string _title{"Select Bot Menu"};
    std::vector<std::string> _bots;
    std::function<void(std ::string const &)> _watch_bot;
};

class TrainMenu : public Menu
{
  public:
    explicit TrainMenu(MenuFactory &factory);
    [[nodiscard]] auto title() -> std::string const & override;
    [[nodiscard]] auto options() -> std::vector<std::string> const & override;
    [[nodiscard]] auto next(int choice) -> std::unique_ptr<Menu> override;

  private:
    MenuFactory &_factory;
    std::string _title{"Train Menu"};
    std::vector<std::string> _options{"Enter Parameters", "Back to MainMenu"};
};

class ParametersMenu : public Menu
{
  public:
    ParametersMenu(MenuFactory &factory, std::vector<std ::string> parameters,
                   std::function<void(std ::string const &)> train_bot);
    [[nodiscard]] std::string const &title() override;
    [[nodiscard]] std::vector<std::string> const &options() override;
    [[nodiscard]] std::unique_ptr<Menu> next(int choice) override;

  private:
    MenuFactory &_factory;
    std::string _title{"Parameters Menu"};
    std::vector<std::string> _parameters;
    std::function<void(std ::string const &)> _train_bot;
};

class TrainingSetupMenu : public Menu
{
  public:
    using TrainCallback =
        std::function<void(TrainingConfig const &, std::shared_ptr<TrainingProgress>)>;

    TrainingSetupMenu(MenuFactory &factory, TrainingConfig config,
                      TrainCallback callback);
    [[nodiscard]] auto title() -> std::string const & override;
    [[nodiscard]] auto options() -> std::vector<std::string> const & override;
    [[nodiscard]] auto next(int choice) -> std::unique_ptr<Menu> override;

    [[nodiscard]] auto get_config() const -> TrainingConfig const &;
    void set_config(TrainingConfig const &config);
    [[nodiscard]] auto get_train_callback() const -> TrainCallback const &;

  private:
    MenuFactory &_factory;
    std::string _title{"Training Setup"};
    std::vector<std::string> _options{"Start Training", "Back to Main Menu"};
    TrainingConfig _config;
    TrainCallback _train_callback;
};

class TrainingActiveMenu : public Menu
{
  public:
    TrainingActiveMenu(MenuFactory &factory,
                       std::shared_ptr<TrainingProgress> progress);
    [[nodiscard]] auto title() -> std::string const & override;
    [[nodiscard]] auto options() -> std::vector<std::string> const & override;
    [[nodiscard]] auto next(int choice) -> std::unique_ptr<Menu> override;
    [[nodiscard]] auto get_progress() const -> std::shared_ptr<TrainingProgress>;

  private:
    MenuFactory &_factory;
    std::string _title{"Training in Progress"};
    std::vector<std::string> _options{"Cancel Training"};
    std::shared_ptr<TrainingProgress> _progress;
};
