#pragma once

#include <compare>
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
    [[nodiscard]] static auto create(Game &game) -> GameState;
    auto operator<=>(GameState const &) const        = default;
    auto operator==(GameState const &) const -> bool = default;
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
    [[nodiscard]] auto get_state() const -> GameState const & override;
    [[nodiscard]] auto get_reward() const -> double override;
    [[nodiscard]] auto has_terminated() const -> bool override;

  private:
    double _reward{0};
    std::unique_ptr<Game> _game;
    GameState _state{};
    void update_game_state();
    bool _alive{true};
};
