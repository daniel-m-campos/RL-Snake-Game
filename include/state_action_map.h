#ifndef RLSNAKEGAME_STATEACTIONMAP_H
#define RLSNAKEGAME_STATEACTIONMAP_H

#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>

template <typename S, typename A>
class StateActionMap {
 public:
  virtual ~StateActionMap() = default;
  virtual const std::vector<S>& GetStates() = 0;
  virtual const std::vector<A>& GetActions(S) = 0;
};

template <typename S, typename A>
class StateActionHashMap : public StateActionMap<S, A> {
 public:
  using state_action_map = std::unordered_map<S, std::vector<A>>;
  StateActionHashMap() = delete;
  explicit StateActionHashMap(state_action_map);
  const std::vector<S>& GetStates() override;
  const std::vector<A>& GetActions(S state) override;

 private:
  state_action_map _state_actions;
  std::vector<S> _states;
};

template <typename S, typename A>
class SimpleStateActionMap : public StateActionMap<S, A> {
 public:
  SimpleStateActionMap() = delete;
  explicit SimpleStateActionMap(std::set<S> states, std::set<A> actions);
  const std::vector<S>& GetStates() override;
  const std::vector<A>& GetActions(S state) override;

 private:
  std::set<S> _states;
  std::vector<S> _unique_states;
  std::set<A> _actions;
  std::vector<A> _unique_actions;
};

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

#endif  // RLSNAKEGAME_STATEACTIONMAP_H
