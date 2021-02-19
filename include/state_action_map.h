#ifndef RLSNAKEGAME_STATEACTIONMAP_H
#define RLSNAKEGAME_STATEACTIONMAP_H

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

#endif  // RLSNAKEGAME_STATEACTIONMAP_H
