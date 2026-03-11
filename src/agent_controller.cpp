#include "agent_controller.h"

#include <SDL_events.h>
#include <chrono>
#include <memory>
#include <thread>

#include "SDL.h" // IWYU pragma: keep
#include "agent_factory.h"
#include "game.h"
#include "game_environment.h"
#include "io.h"

namespace
{
double constexpr epsilon{0.01};
double constexpr discount_factor{0.9};
double constexpr step_size{0.5};
int constexpr initial_grid_x{8};
int constexpr initial_grid_y{8};
int constexpr sleep_ms{50};
} // namespace

AgentController::AgentController(std::string const &filename)
{
    auto unique_valuer = io::load<GameState, snake::Direction>(filename);
    std::shared_ptr<ActionValuer<GameState, snake::Direction>> action_valuer{
        std::move(unique_valuer)};
    _agent = AgentFactory<GameState, snake::Direction>::create_q_agent(
        action_valuer, epsilon, discount_factor, step_size,
        GameState{{{initial_grid_x, initial_grid_y}}}, snake::Direction::kUp);
}

bool AgentController::update(Game &game)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    auto state          = GameState::create(game);
    auto action         = _agent->get_action(state);
    snake::Snake &snake = game.get_snake();
    snake.set_speed(1.0F);
    snake.set_direction(action);
    return snake.is_alive();
}
