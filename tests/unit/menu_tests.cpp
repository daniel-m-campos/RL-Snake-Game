#include "gtest/gtest.h"
#include "menu.h"

class MockMenuFactory : public MenuFactory {
 public:
  std::unique_ptr<Menu> CreateMainMenu() override {
    return std::make_unique<MainMenu>(this, []() {});
  }
  std::unique_ptr<Menu> CreateWatchMenu() override {
    return std::make_unique<WatchMenu>(this);
  }
  std::unique_ptr<Menu> CreateTrainMenu() override {
    return std::make_unique<TrainMenu>(this);
  }
  std::unique_ptr<Menu> CreateSelectBotMenu() override {
    return std::make_unique<SelectBotMenu>(
        this, std::vector<std::string>{"0", "1", "2"},
        [](auto& bot) { std::cout << "Select Bot: " << bot << "\n"; });
  }
  std::unique_ptr<Menu> CreateParametersMenu() override {
    return std::make_unique<ParametersMenu>(
        this, std::vector<std::string>{"8x8", "16x16", "24x24", "32x32"},
        [](auto& bot) { std::cout << "Train Bot: " << bot << "\n"; });
  }
};

class MenuTestFixture : public ::testing::Test {
 public:
  MockMenuFactory factory;
};

TEST_F(MenuTestFixture, MainMenuTransitions) {
  auto menu = factory.CreateMainMenu();
  EXPECT_EQ(menu->Title(), "Main Menu");
  auto next_menu = menu->Next(0);
  EXPECT_EQ(next_menu->Title(), "Main Menu");
  next_menu = menu->Next(1);
  EXPECT_EQ(next_menu->Title(), "Watch Menu");
  next_menu = menu->Next(2);
  EXPECT_EQ(next_menu->Title(), "Train Menu");
  next_menu = menu->Next(3);
  EXPECT_EQ(next_menu, nullptr);
}

TEST_F(MenuTestFixture, WatchMenuTransitions) {
  auto menu = factory.CreateWatchMenu();
  EXPECT_EQ(menu->Title(), "Watch Menu");
  auto next_menu = menu->Next(0);
  EXPECT_EQ(next_menu->Title(), "Select Bot Menu");
  next_menu = menu->Next(1);
  EXPECT_EQ(next_menu->Title(), "Main Menu");
  next_menu = menu->Next(2);
  EXPECT_EQ(next_menu, nullptr);
}

TEST_F(MenuTestFixture, SelectBotMenuTransitions) {
  auto menu = factory.CreateSelectBotMenu();
  EXPECT_EQ(menu->Title(), "Select Bot Menu");
  std::unique_ptr<Menu> next_menu;
  for (int i = 0; i < 3; ++i) {
    next_menu = menu->Next(i);
    EXPECT_EQ(next_menu->Title(), "Main Menu");
  }
  next_menu = menu->Next(3);
  EXPECT_EQ(next_menu, nullptr);
}

TEST_F(MenuTestFixture, TrainMenuTransitions) {
  auto menu = factory.CreateTrainMenu();
  EXPECT_EQ(menu->Title(), "Train Menu");
  auto next_menu = menu->Next(0);
  EXPECT_EQ(next_menu->Title(), "Parameters Menu");
  next_menu = menu->Next(1);
  EXPECT_EQ(next_menu->Title(), "Main Menu");
  next_menu = menu->Next(2);
  EXPECT_EQ(next_menu, nullptr);
}

TEST_F(MenuTestFixture, ParametersMenuTransitions) {
  auto menu = factory.CreateParametersMenu();
  EXPECT_EQ(menu->Title(), "Parameters Menu");
  std::unique_ptr<Menu> next_menu;
  for (int i = 0; i < 4; ++i) {
    next_menu = menu->Next(i);
    EXPECT_EQ(next_menu->Title(), "Main Menu");
  }
  next_menu = menu->Next(5);
  EXPECT_EQ(next_menu, nullptr);
}