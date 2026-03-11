#include "game_menu_factory.h"
#include "gui.h"
#include <cstdlib>
#include <memory>

auto main() -> int
{
    GUI gui{std::make_unique<GameMenuFactory>()};
    gui.show();
    return EXIT_SUCCESS;
}
