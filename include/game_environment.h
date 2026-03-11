#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <ostream>
#include <vector>

#include "environment.h"
#include "game.h"
#include "hash_util.h"
#include "snake.h"

struct GameState
{
    std::vector<snake::Point<int>> body_to_food;
    static auto create(Game &game) -> GameState;
    auto operator==(GameState const &rhs) const -> bool;
    auto operator!=(GameState const &rhs) const -> bool;
    auto operator<(GameState const &rhs) const -> bool;
    auto operator>(GameState const &rhs) const -> bool;
    auto operator<=(GameState const &rhs) const -> bool;
    auto operator>=(GameState const &rhs) const -> bool;
    friend auto operator<<(std::ostream &os,
                           GameState const &state)
        -> std::ostream &; // NOLINT(google-objc-function-naming)
};

template <> struct std::hash<GameState>
{
    auto operator()(GameState const &state) const -> size_t
    {
        size_t seed = 0;
        for (auto const &part : state.body_to_food)
        {
            ::hash_combine(seed, part.x);
            ::hash_combine(seed, part.y);
        }
        return seed;
    }
};

class GameEnvironment : public Environment<GameState, snake::Direction>
{
  public:
    explicit GameEnvironment(std::unique_ptr<Game> game);
    void update(snake::Direction const &action) override;
    auto get_state() -> GameState const & override;
    auto get_reward() -> double override;
    auto has_terminated() -> bool override;

  private:
    double _reward{0};
    std::unique_ptr<Game> _game;
    GameState _state{};
    void update_game_state();
    bool _alive{true};
};
