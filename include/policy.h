#ifndef RLSNAKEGAME_POLICY_H
#define RLSNAKEGAME_POLICY_H

#include <algorithm>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

#include "action_valuer.h"
#include "hash_util.h"

template <typename S, typename A>
class Policy {
 public:
  virtual ~Policy() = default;
  virtual A operator()(S) = 0;
  virtual double Probability(A, S) = 0;
};

template <typename S, typename A>
class EpsilonGreedy : public Policy<S, A> {
 public:
  using state_action_map = std::unordered_map<S, std::vector<A>>;

  EpsilonGreedy() = delete;
  EpsilonGreedy(std::shared_ptr<ActionValuer<S, A>> _action_valuer,
                double epsilon);
  EpsilonGreedy(state_action_map state_actions, double epsilon);
  A operator()(S state) override;
  double Probability(A action, S given_state) override;

 private:
  double _epsilon;
  std::shared_ptr<ActionValuer<S, A>> _action_valuer;
  std::mt19937_64 _engine;
  std::uniform_real_distribution<> _uniform_dist;
};

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(
    std::shared_ptr<ActionValuer<S, A>> action_valuer, double epsilon)
    : _epsilon(epsilon), _action_valuer(action_valuer) {}

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(
    EpsilonGreedy::state_action_map state_actions, double epsilon)
    : _epsilon(epsilon),
      _uniform_dist(std::uniform_real_distribution<>(0, 1)),
      _engine(std::random_device()()) {
  _action_valuer = std::make_unique<SimpleActionValuer<S, A>>(
      std::make_unique<StateActionHashMap<S, A>>(state_actions));
}

template <typename S, typename A>
double EpsilonGreedy<S, A>::Probability(A action, S state) {
  auto actions = _action_valuer->ArgMax(state);
  double num_actions =
      _action_valuer->GetStateActionMap().GetActions(state).size();
  if (std::find(actions.begin(), actions.end(), action) != actions.end()) {
    return _epsilon / num_actions + (1 - _epsilon) / actions.size();
  } else {
    return _epsilon / num_actions;
  }
}

template <typename S, typename A>
A EpsilonGreedy<S, A>::operator()(S state) {
  std::vector<A> actions;
  if (_uniform_dist(_engine) < _epsilon) {
    actions = _action_valuer->GetStateActionMap().GetActions(state);
  } else {
    actions = _action_valuer->ArgMax(state);
  }
  std::shuffle(actions.begin(), actions.end(), _engine);
  return actions[0];
}
#endif  // RLSNAKEGAME_POLICY_H
