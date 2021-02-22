#ifndef RLSNAKEGAME_GAME_ENVIRONMENT_H
#define RLSNAKEGAME_GAME_ENVIRONMENT_H

#include <memory>
#include <unordered_map>

#include "environment.h"
#include "game.h"
#include "hash_util.h"
#include "snake.h"

struct GameState {
  snake::Point<int> food;
  snake::Point<int> tail;
  bool operator==(const GameState& rhs) const;
  bool operator!=(const GameState& rhs) const;
  bool operator<(const GameState& rhs) const;
  bool operator>(const GameState& rhs) const;
  bool operator<=(const GameState& rhs) const;
  bool operator>=(const GameState& rhs) const;
  static GameState Create(Game& game);
};

template <>
struct std::hash<GameState> {
  inline size_t operator()(const GameState& state) const {
    size_t seed = 0;
    ::hash_combine(seed, state.food.x);
    ::hash_combine(seed, state.food.y);
    ::hash_combine(seed, state.tail.x);
    ::hash_combine(seed, state.tail.y);
    return seed;
  }
};

class GameEnvironment : public Environment<GameState, snake::Direction> {
 public:
  explicit GameEnvironment(std::unique_ptr<Game> game);
  void Update(const snake::Direction& direction) override;
  const GameState& GetState() override;
  double GetReward() override;
  bool HasTerminated() override;

 private:
  double _reward{0};
  std::unique_ptr<Game> _game;
  GameState _state{};
  void UpdateGameState();
  bool _alive{true};
};

#endif  // RLSNAKEGAME_GAME_ENVIRONMENT_H
