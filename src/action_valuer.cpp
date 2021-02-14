#include "action_valuer.h"

#include <algorithm>

template <typename S, typename A>
SimpleActionValuer<S, A>::SimpleActionValuer(
    SimpleActionValuer::state_action_map state_actions, double init_value)
    : _state_actions{state_actions} {
  for (const auto& [state, actions] : _state_actions) {
    for (const auto& action : actions) {
      _action_value_map[{state, action}] = init_value;
    }
  }
}

template <typename S, typename A>
A SimpleActionValuer<S, A>::ArgMax(S state) {
  auto compare = [&](const auto& left, const auto& right) {
    return _action_value_map[{state, left}] < _action_value_map[{state, right}];
  };
  auto actions = _state_actions.at(state);
  return *std::max_element(actions.begin(), actions.end(), compare);
}

template <typename S, typename A>
std::vector<S> SimpleActionValuer<S, A>::GetStates() {
  if (_states.empty()) {
    for (const auto& [state, actions] : _state_actions) {
      _states.push_back(state);
      std::sort(_states.begin(), _states.end());
    }
  }
  return _states;
}

template <typename S, typename A>
std::vector<A> SimpleActionValuer<S, A>::GetActions(S state) {
  return _state_actions.at(state);
}

template <typename S, typename A>
double SimpleActionValuer<S, A>::GetValue(S state, A action) {
  return _action_value_map.at({state, action});
}

template <typename S, typename A>
void SimpleActionValuer<S, A>::SetValue(S state, A action, double new_value) {
  _action_value_map.at({state, action}) = new_value;
}

template class SimpleActionValuer<int, int>;