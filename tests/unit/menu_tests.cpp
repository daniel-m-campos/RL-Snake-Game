#include "menu.h"
#include "gtest/gtest.h"

class MockMenuFactory : public MenuFactory
{
  public:
    std::unique_ptr<Menu> create_main_menu() override
    {
        return std::make_unique<MainMenu>(*this, []() {});
    }
    std::unique_ptr<Menu> create_watch_menu() override
    {
        return std::make_unique<WatchMenu>(*this);
    }
    std::unique_ptr<Menu> create_train_menu() override
    {
        return std::make_unique<TrainMenu>(*this);
    }
    std::unique_ptr<Menu> create_select_bot_menu() override
    {
        return std::make_unique<SelectBotMenu>(
            *this, std::vector<std::string>{"0", "1", "2"},
            [](auto &bot) { std::cout << "Select Bot: " << bot << "\n"; });
    }
    std::unique_ptr<Menu> create_parameters_menu() override
    {
        return std::make_unique<ParametersMenu>(
            *this, std::vector<std::string>{"8x8", "16x16", "24x24", "32x32"},
            [](auto &bot) { std::cout << "Train Bot: " << bot << "\n"; });
    }
};

class MenuTestFixture : public ::testing::Test
{
  public:
    MockMenuFactory factory;
};

TEST_F(MenuTestFixture, MainMenuTransitions)
{
    auto menu = factory.create_main_menu();
    EXPECT_EQ(menu->title(), "Main Menu");
    auto next_menu = menu->next(0);
    EXPECT_EQ(next_menu->title(), "Main Menu");
    next_menu = menu->next(1);
    EXPECT_EQ(next_menu->title(), "Watch Menu");
    next_menu = menu->next(2);
    EXPECT_EQ(next_menu->title(), "Train Menu");
    next_menu = menu->next(3);
    EXPECT_EQ(next_menu, nullptr);
}

TEST_F(MenuTestFixture, WatchMenuTransitions)
{
    auto menu = factory.create_watch_menu();
    EXPECT_EQ(menu->title(), "Watch Menu");
    auto next_menu = menu->next(0);
    EXPECT_EQ(next_menu->title(), "Select Bot Menu");
    next_menu = menu->next(1);
    EXPECT_EQ(next_menu->title(), "Main Menu");
    next_menu = menu->next(2);
    EXPECT_EQ(next_menu, nullptr);
}

TEST_F(MenuTestFixture, SelectBotMenuTransitions)
{
    auto menu = factory.create_select_bot_menu();
    EXPECT_EQ(menu->title(), "Select Bot Menu");
    std::unique_ptr<Menu> next_menu;
    for (int i = 0; i < 3; ++i)
    {
        next_menu = menu->next(i);
        EXPECT_EQ(next_menu->title(), "Main Menu");
    }
    next_menu = menu->next(3);
    EXPECT_EQ(next_menu, nullptr);
}

TEST_F(MenuTestFixture, TrainMenuTransitions)
{
    auto menu = factory.create_train_menu();
    EXPECT_EQ(menu->title(), "Train Menu");
    auto next_menu = menu->next(0);
    EXPECT_EQ(next_menu->title(), "Parameters Menu");
    next_menu = menu->next(1);
    EXPECT_EQ(next_menu->title(), "Main Menu");
    next_menu = menu->next(2);
    EXPECT_EQ(next_menu, nullptr);
}

TEST_F(MenuTestFixture, ParametersMenuTransitions)
{
    auto menu = factory.create_parameters_menu();
    EXPECT_EQ(menu->title(), "Parameters Menu");
    std::unique_ptr<Menu> next_menu;
    for (int i = 0; i < 4; ++i)
    {
        next_menu = menu->next(i);
        EXPECT_EQ(next_menu->title(), "Main Menu");
    }
    next_menu = menu->next(5);
    EXPECT_EQ(next_menu, nullptr);
}
