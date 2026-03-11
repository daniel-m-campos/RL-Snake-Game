#include "menu.h"
#include "training_config.h"

#include <memory>

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
    auto create_training_setup_menu() -> std::unique_ptr<Menu> override
    {
        return std::make_unique<TrainingSetupMenu>(
            *this, TrainingConfig{},
            [](auto const & /*config*/, auto /*progress*/) {});
    }
    auto create_training_active_menu(std::shared_ptr<TrainingProgress> progress)
        -> std::unique_ptr<Menu> override
    {
        return std::make_unique<TrainingActiveMenu>(*this, std::move(progress));
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

TEST_F(MenuTestFixture, TrainingSetupMenuOptions)
{
    auto menu = factory.create_training_setup_menu();
    EXPECT_EQ(menu->title(), "Training Setup");
    EXPECT_EQ(menu->options().size(), 2u);
}

TEST_F(MenuTestFixture, TrainingSetupMenuBackReturnsMainMenu)
{
    auto menu = factory.create_training_setup_menu();
    auto next = menu->next(1);
    ASSERT_NE(next, nullptr);
    EXPECT_EQ(next->title(), "Main Menu");
}

TEST_F(MenuTestFixture, TrainingSetupMenuConfigAccess)
{
    auto menu  = factory.create_training_setup_menu();
    auto *setup = dynamic_cast<TrainingSetupMenu *>(menu.get());
    ASSERT_NE(setup, nullptr);
    EXPECT_EQ(setup->get_config().num_episodes, 1000);
    TrainingConfig c;
    c.num_episodes = 500;
    setup->set_config(c);
    EXPECT_EQ(setup->get_config().num_episodes, 500);
}

TEST_F(MenuTestFixture, TrainingActiveMenuCancellation)
{
    auto progress = std::make_shared<TrainingProgress>();
    auto menu     = factory.create_training_active_menu(progress);
    EXPECT_EQ(menu->title(), "Training in Progress");
    auto next = menu->next(0);
    EXPECT_TRUE(progress->cancel_requested.load());
    ASSERT_NE(next, nullptr);
    EXPECT_EQ(next->title(), "Main Menu");
}
