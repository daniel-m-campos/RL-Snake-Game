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
  // FIXME: This needs to account for the GreedyAction
  return 1 / _epsilon;
}

template <typename S, typename A>
double EpsilonGreedy<S, A>::Value(A action, S state) {
  return 0;
}

template <typename S, typename A>
A EpsilonGreedy<S, A>::operator()(S state) {
  std::vector<A> greedy_actions = GreedyAction(state);
  if (_uniform_dist(_engine) < _epsilon) {
    std::shuffle(greedy_actions.begin(), greedy_actions.end(), _engine);
  }
  return greedy_actions[0];
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
  std::vector<A> greedy_actions = GetActions(state);
  std::sort(greedy_actions.begin(), greedy_actions.end(),
            [&](auto& left, auto& right) {
              return _action_values[std::make_pair(state, left)] >
                     _action_values[std::make_pair(state, right)];
            });
  const auto max_value =
      _action_values[std::make_pair(state, greedy_actions[0])];
  auto iter = greedy_actions.begin();
  while (iter != greedy_actions.end()) {
    if (_action_values[std::make_pair(state, *iter)] != max_value) {
      iter = greedy_actions.erase(iter);
    } else {
      ++iter;
    }
  }
  return greedy_actions;
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

template class EpsilonGreedy<int, int>;
