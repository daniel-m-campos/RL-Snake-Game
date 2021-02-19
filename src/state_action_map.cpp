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

template <typename S, typename A>
SimpleStateActionMap<S, A>::SimpleStateActionMap(std::set<S> states,
                                                 std::set<A> actions)
    : _states{states}, _actions{actions} {
  std::copy(_actions.begin(), _actions.end(),
            std::back_inserter(_unique_actions));
}

template <typename S, typename A>
const std::vector<A>& SimpleStateActionMap<S, A>::GetActions(S state) {
  _states.insert(state);
  return _unique_actions;
}

template <typename S, typename A>
const std::vector<S>& SimpleStateActionMap<S, A>::GetStates() {
  _unique_states.clear();
  std::copy(_states.begin(), _states.end(), std::back_inserter(_unique_states));
  return _unique_states;
}

template class StateActionHashMap<int, int>;
template class SimpleStateActionMap<int, int>;