#include "game_environment.h"

#include "game.h"
#include "snake.h"
#include <memory>
#include <ostream>
#include <tuple>
#include <utility>
#include <vector>

namespace
{
double constexpr death_reward{-100};
double constexpr movement_penalty{-0.1};
double constexpr feed_reward{1.0};
} // namespace

GameEnvironment::GameEnvironment(std::unique_ptr<Game> game) : _game{std::move(game)}
{
    update_game_state();
}

auto GameState::operator==(GameState const &rhs) const -> bool
{
    return std::tie(body_to_food) == std::tie(rhs.body_to_food);
}

auto GameState::operator!=(GameState const &rhs) const -> bool
{
    return !(rhs == *this);
}

auto GameState::operator<(GameState const &rhs) const -> bool
{
    return std::tie(body_to_food) < std::tie(rhs.body_to_food);
}

auto GameState::operator>(GameState const &rhs) const -> bool
{
    return rhs < *this;
}

auto GameState::operator<=(GameState const &rhs) const -> bool
{
    return !(rhs < *this);
}

auto GameState::operator>=(GameState const &rhs) const -> bool
{
    return !(*this < rhs);
}

auto GameState::create(Game &game) -> GameState
{
    auto food = game.get_food().get_location();
    std::vector<snake::Point<int>> body_to_food;
    body_to_food.push_back(
        {static_cast<int>(static_cast<float>(food.x) - game.get_snake().get_head_x()),
         static_cast<int>(static_cast<float>(food.y) - game.get_snake().get_head_y())});
    for (auto const &part : game.get_snake().get_body())
    {
        body_to_food.push_back({food.x - part.x, food.y - part.y});
    }
    return GameState{body_to_food};
}

auto operator<<(std::ostream &os, GameState const &state) -> std::ostream &
{
    for (auto const &part : state.body_to_food)
    {
        os << part.x << ",";
        os << part.y << ",";
    }
    return os;
}

void GameEnvironment::update(snake::Direction const &action)
{
    if (!_alive)
    {
        _reward = 0;
        return;
    }
    auto score = _game->get_score();
    _game->get_snake().set_direction(action);
    _game->get_snake().set_speed(1.0F);
    _game->update();
    update_game_state();
    if (!_game->get_snake().is_alive())
    {
        _alive  = false;
        _reward = death_reward;
    }
    else if (_game->get_score() > score)
    {
        _reward = feed_reward;
    }
    else
    {
        _reward = movement_penalty;
    }
}

void GameEnvironment::update_game_state()
{
    auto const &food = _game->get_food().get_location();
    _state           = GameState::create(*_game);
}

GameState const &GameEnvironment::get_state()
{
    return _state;
}

auto GameEnvironment::get_reward() -> double
{
    return _reward;
}

auto GameEnvironment::has_terminated() -> bool
{
    return !_alive;
}
