#include "policy.h"

#include <algorithm>

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(
    std::unique_ptr<ActionValuer<S, A>> action_valuer, double epsilon)
    : _epsilon(epsilon), _action_valuer(std::move(action_valuer)) {}

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(
    EpsilonGreedy::state_action_map state_actions, double epsilon)
    : _epsilon(epsilon),
      _uniform_dist(std::uniform_real_distribution<>(0, 1)),
      _engine(std::random_device()()) {
  _action_valuer = std::make_unique<SimpleActionValuer<S, A>>(state_actions);
}

template <typename S, typename A>
double EpsilonGreedy<S, A>::Probability(A action, S state) {
  auto actions = _action_valuer->ArgMax(state);
  double num_actions = _action_valuer->GetActions(state).size();
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
    actions = _action_valuer->GetStates();
  } else {
    actions = _action_valuer->ArgMax(state);
  }
  std::shuffle(actions.begin(), actions.end(), _engine);
  return actions[0];
}

template class EpsilonGreedy<int, int>;
