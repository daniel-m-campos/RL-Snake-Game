#include "policy.h"

#include <algorithm>
#include <list>
#include <numeric>

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(size_t num_states, size_t num_actions,
                                   double epsilon)
    : _epsilon(epsilon),
      _num_states(num_states),
      _num_actions(num_actions),
      _engine(std::random_device()()),
      _uniform_dist(std::uniform_real_distribution<>(0, 1)) {
  CacheActions();
  std::vector<A> actions(num_actions);
  std::iota(actions.begin(), actions.end(), 0);
  for (S state = 0; state < num_states; ++state) {
    _state_actions[state] = actions;
  }
}

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(
    EpsilonGreedy::state_action_map state_actions, double epsilon)
    : _epsilon(epsilon),
      _state_actions(state_actions),
      _uniform_dist(std::uniform_real_distribution<>(0, 1)),
      _engine(std::random_device()()) {
  CacheActions();
  _num_actions = _actions.size();
}

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(
    EpsilonGreedy::state_action_map state_actions,
    EpsilonGreedy::action_value_map action_values, double epsilon)
    : _epsilon(epsilon),
      _num_states(state_actions.size()),
      _action_values(action_values),
      _state_actions(state_actions),
      _uniform_dist(std::uniform_real_distribution<>(0, 1)),
      _engine(std::random_device()()) {
  CacheActions();
  _num_actions = _actions.size();
}

template <typename S, typename A>
void EpsilonGreedy<S, A>::CacheActions() {
  for (const auto& [state, actions] : _state_actions) {
    for (const auto& action : actions) {
      _actions[action] = true;
    }
  }
}

template <typename S, typename A>
double EpsilonGreedy<S, A>::Probability(A action, S state) {
  auto actions = GreedyAction(state);
  double num_actions = _state_actions[state].size();
  if (std::find(actions.begin(), actions.end(), action) != actions.end()) {
    return _epsilon / num_actions + (1 - _epsilon) / actions.size();
  } else {
    return _epsilon / num_actions;
  }
}

template <typename S, typename A>
double EpsilonGreedy<S, A>::GetValue(S state, A action) {
  return _action_values[{state, action}];
}

template <typename S, typename A>
A EpsilonGreedy<S, A>::operator()(S state) {
  std::vector<A> actions;
  if (_uniform_dist(_engine) < _epsilon) {
    actions = _state_actions[state];
  } else {
    actions = GreedyAction(state);
  }
  std::shuffle(actions.begin(), actions.end(), _engine);
  return actions[0];
}

template <typename S, typename A>
size_t EpsilonGreedy<S, A>::StateSize() const {
  return _num_states;
}

template <typename S, typename A>
size_t EpsilonGreedy<S, A>::ActionSize() const {
  return _num_actions;
}

template <typename S, typename A>
std::vector<A> EpsilonGreedy<S, A>::GreedyAction(S state) {
  auto actions = GetActions(state);
  std::sort(
      actions.begin(), actions.end(), [&](const auto& left, const auto& right) {
        return _action_values[{state, left}] > _action_values[{state, right}];
      });
  const auto max_value = _action_values[{state, actions[0]}];
  auto iter = actions.begin();
  while (iter != actions.end()) {
    if (_action_values[{state, *iter}] != max_value) {
      iter = actions.erase(iter);
    } else {
      ++iter;
    }
  }
  return actions;
}

template <typename S, typename A>
std::vector<A> EpsilonGreedy<S, A>::GetActions(const S& state) {
  std::vector<A> greedy_actions;
  greedy_actions.reserve(_state_actions[state].size());
  for (const auto& action : _state_actions[state]) {
    greedy_actions.push_back(action);
  }
  return greedy_actions;
}

template <typename S, typename A>
void EpsilonGreedy<S, A>::SetValue(S state, A action, double value) {
  _action_values[{state, action}] = value;
}

template class EpsilonGreedy<int, int>;
