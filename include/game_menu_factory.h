#pragma once

#include "menu.h"
#include <cstddef>
#include <memory>
#include <utility>

class GameMenuFactory : public MenuFactory
{
  public:
    auto create_main_menu() -> std::unique_ptr<Menu> override;
    auto create_watch_menu() -> std::unique_ptr<Menu> override;
    auto create_train_menu() -> std::unique_ptr<Menu> override;
    auto create_select_bot_menu() -> std::unique_ptr<Menu> override;
    auto create_parameters_menu() -> std::unique_ptr<Menu> override;

  private:
    static auto get_grid_size(std::string const &filename) -> std::pair<size_t, size_t>;
};
