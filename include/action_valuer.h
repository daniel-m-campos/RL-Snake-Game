#ifndef RLSNAKEGAME_ACTION_VALUER_H
#define RLSNAKEGAME_ACTION_VALUER_H

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "hash_util.h"
#include "state_action_map.h"

template <typename S, typename A>
class ActionValuer {
 public:
  virtual ~ActionValuer() = default;
  virtual double GetValue(S, A) = 0;
  virtual void SetValue(S, A, double) = 0;
  virtual std::vector<A> ArgMax(S) = 0;
  virtual StateActionMap<S, A>& GetStateActionMap() = 0;
};

template <typename S, typename A>
class SimpleActionValuer : public ActionValuer<S, A> {
 public:
  using action_value_map = std::unordered_map<std::pair<S, A>, double>;

  SimpleActionValuer() = delete;
  explicit SimpleActionValuer(
      std::unique_ptr<StateActionMap<S, A>> state_action_map,
      action_value_map action_value_map);
  explicit SimpleActionValuer(
      std::unique_ptr<StateActionMap<S, A>> state_action_map,
      double init_value = 0);
  std::vector<A> ArgMax(S state) override;
  double GetValue(S state, A action) override;
  void SetValue(S state, A action, double new_value) override;
  StateActionMap<S, A>& GetStateActionMap() override;

 private:
  std::unique_ptr<StateActionMap<S, A>> _state_action_map;
  action_value_map _action_value_map;
};

template <typename S, typename A>
SimpleActionValuer<S, A>::SimpleActionValuer(
    std::unique_ptr<StateActionMap<S, A>> state_action_map, double init_value)
    : _state_action_map{std::move(state_action_map)} {
  for (const auto& state : _state_action_map->GetStates()) {
    for (const auto& action : _state_action_map->GetActions(state)) {
      _action_value_map[{state, action}] = init_value;
    }
  }
}

template <typename S, typename A>
SimpleActionValuer<S, A>::SimpleActionValuer(
    std::unique_ptr<StateActionMap<S, A>> state_action_map,
    SimpleActionValuer::action_value_map action_value_map)
    : _state_action_map{std::move(state_action_map)},
      _action_value_map{action_value_map} {}

template <typename S, typename A>
std::vector<A> SimpleActionValuer<S, A>::ArgMax(S state) {
  auto compare = [&](const auto& left, const auto& right) {
    return _action_value_map[{state, left}] > _action_value_map[{state, right}];
  };
  auto actions = _state_action_map->GetActions(state);
  std::sort(actions.begin(), actions.end(), compare);
  const auto max_value = _action_value_map[{state, actions[0]}];
  auto iter = actions.begin();
  while (iter != actions.end()) {
    if (_action_value_map[{state, *iter}] != max_value) {
      iter = actions.erase(iter);
    } else {
      ++iter;
    }
  }
  return actions;
}

template <typename S, typename A>
double SimpleActionValuer<S, A>::GetValue(S state, A action) {
  return _action_value_map[{state, action}];
}

template <typename S, typename A>
void SimpleActionValuer<S, A>::SetValue(S state, A action, double new_value) {
  _action_value_map[{state, action}] = new_value;
}

template <typename S, typename A>
StateActionMap<S, A>& SimpleActionValuer<S, A>::GetStateActionMap() {
  return *_state_action_map;
}

#endif  // RLSNAKEGAME_ACTION_VALUER_H
