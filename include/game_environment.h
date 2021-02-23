#ifndef RLSNAKEGAME_GAME_ENVIRONMENT_H
#define RLSNAKEGAME_GAME_ENVIRONMENT_H

#include <memory>
#include <unordered_map>

#include "environment.h"
#include "game.h"
#include "hash_util.h"
#include "snake.h"

struct GameState {
  std::vector<snake::Point<int>> body_to_food;
  static GameState Create(Game& game);
  bool operator==(const GameState& rhs) const;
  bool operator!=(const GameState& rhs) const;
  bool operator<(const GameState& rhs) const;
  bool operator>(const GameState& rhs) const;
  bool operator<=(const GameState& rhs) const;
  bool operator>=(const GameState& rhs) const;
};

template <>
struct std::hash<GameState> {
  inline size_t operator()(const GameState& state) const {
    size_t seed = 0;
    for (const auto& part : state.body_to_food) {
      ::hash_combine(seed, part.x);
      ::hash_combine(seed, part.y);
    }
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
