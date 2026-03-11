#include "game_menu_factory.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "agent_controller.h"
#include "io.h"
#include "keyboard_controller.h"
#include "main_utils.h"
#include "menu.h"
#include "trainer.h"

auto GameMenuFactory::create_main_menu() -> std::unique_ptr<Menu>
{
    return std::make_unique<MainMenu>(this,
                                      []()
                                      {
                                          auto controller = KeyboardController();
                                          play(controller);
                                      });
}

auto GameMenuFactory::create_watch_menu() -> std::unique_ptr<Menu>
{
    return std::make_unique<WatchMenu>(this);
}

auto GameMenuFactory::create_train_menu() -> std::unique_ptr<Menu>
{
    return std::make_unique<TrainMenu>(this);
}

auto GameMenuFactory::create_select_bot_menu() -> std::unique_ptr<Menu>
{
    auto choices             = io::find_files();
    std::string const suffix = io::file_suffix;
    for (auto &file : choices)
    {
        file = std::regex_replace(file, std::regex(suffix), "");
    }
    auto watch_bot = [suffix](std::string const &choice)
    {
        std::string filename = choice + suffix;
        auto controller      = AgentController(filename);
        auto [width, height] = get_grid_size(filename);
        play(controller, width, height);
    };
    return std::make_unique<SelectBotMenu>(this, choices, watch_bot);
}

auto GameMenuFactory::create_parameters_menu() -> std::unique_ptr<Menu>
{
    auto train_bot = [](std ::string const &choice)
    {
        auto [width, height] = get_grid_size(choice);
        int64_t constexpr training_episodes{100'000};
        train(width, height, training_episodes);
    };
    return std::make_unique<ParametersMenu>(
        this, std::vector<std::string>{"8x8", "16x16", "24x24", "32x32"}, train_bot);
}

auto GameMenuFactory::get_grid_size(std::string const &filename)
    -> std::pair<size_t, size_t>
{
    std::istringstream stream{filename};
    size_t width{0};
    size_t height{0};
    char separator{};
    if (stream >> width >> separator >> height)
    {
        return std::pair<size_t, size_t>(width, height);
    }
    size_t constexpr default_grid_size{8};
    return std::pair<size_t, size_t>(default_grid_size, default_grid_size);
}
