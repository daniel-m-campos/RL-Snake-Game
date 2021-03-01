#include "game_menu_factory.h"

#include <memory>
#include <regex>

#include "agent_controller.h"
#include "gui.h"
#include "io.h"
#include "keyboard_controller.h"
#include "main_utils.h"
#include "trainer.h"

std::unique_ptr<Menu> GameMenuFactory::CreateMainMenu() {
  return std::make_unique<MainMenu>(this, []() {
    auto controller = KeyboardController();
    Play(controller);
  });
}

std::unique_ptr<Menu> GameMenuFactory::CreateWatchMenu() {
  return std::make_unique<WatchMenu>(this);
}

std::unique_ptr<Menu> GameMenuFactory::CreateTrainMenu() {
  return std::make_unique<TrainMenu>(this);
}

std::unique_ptr<Menu> GameMenuFactory::CreateSelectBotMenu() {
  auto choices = io::FindFiles();
  const std::string suffix = "_action_valuer.txt";
  for (auto& file : choices) {
    file = std::regex_replace(file, std::regex(suffix), "");
  }
  auto watch_bot = [suffix](const std::string& choice) {
    std::string filename = choice + suffix;
    auto controller = AgentController(filename);
    auto [width, height] = GetGridSize(filename);
    Play(controller, width, height);
  };
  return std::make_unique<SelectBotMenu>(this, choices, watch_bot);
}

std::unique_ptr<Menu> GameMenuFactory::CreateParametersMenu() {
  auto train_bot = [](const std ::string& choice) {
    auto [width, height] = GetGridSize(choice);
    Train(width, height, 5'000);
  };
  return std::make_unique<ParametersMenu>(
      this, std::vector<std::string>{"8x8", "16x16", "24x24", "32x32"},
      train_bot);
}

std::pair<size_t, size_t> GameMenuFactory::GetGridSize(
    const std::string& filename) {
  std::istringstream stream{filename};
  size_t width, height;
  char separator;
  if (stream >> width >> separator >> height)
    return std::pair<size_t, size_t>(width, height);
  return std::pair<size_t, size_t>(8, 8);
}
