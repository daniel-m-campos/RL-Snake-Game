#include "state_action_map.h"

#include <algorithm>

template <typename S, typename A>
StateActionHashMap<S, A>::StateActionHashMap(
    StateActionHashMap::state_action_map state_action_map)
    : _state_actions{state_action_map} {}

template <typename S, typename A>
const std::vector<S>& StateActionHashMap<S, A>::GetStates() {
  if (_states.empty()) {
    for (const auto& [state, actions] : _state_actions) {
      _states.push_back(state);
      std::sort(_states.begin(), _states.end());
    }
  }
  return _states;
}

template <typename S, typename A>
const std::vector<A>& StateActionHashMap<S, A>::GetActions(S state) {
  return _state_actions.at(state);
}

template class StateActionHashMap<int, int>;