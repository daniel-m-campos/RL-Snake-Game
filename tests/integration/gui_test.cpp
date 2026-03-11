#include "gui.h"

#include <iostream>
#include <memory>
#include <vector>

#include "menu.h"

class TestMenuFactory : public MenuFactory
{
  public:
    std::unique_ptr<Menu> create_main_menu() override
    {
        return std::make_unique<MainMenu>(this, []() {});
    }
    std::unique_ptr<Menu> create_watch_menu() override
    {
        return std::make_unique<WatchMenu>(this);
    }
    std::unique_ptr<Menu> create_train_menu() override
    {
        return std::make_unique<TrainMenu>(this);
    }
    std::unique_ptr<Menu> create_select_bot_menu() override
    {
        return std::make_unique<SelectBotMenu>(
            this, std::vector<std::string>{"0", "1", "2"},
            [](auto &bot) { std::cout << "Select Bot: " << bot << "\n"; });
    }
    std::unique_ptr<Menu> create_parameters_menu() override
    {
        return std::make_unique<ParametersMenu>(
            this, std::vector<std::string>{"8x8", "16x16", "24x24", "32x32"},
            [](auto &bot) { std::cout << "Train Bot: " << bot << "\n"; });
    }
};

int main()
{
    GUI gui{std::make_unique<TestMenuFactory>()};
    gui.show();
}
