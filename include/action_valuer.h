#ifndef RLSNAKEGAME_ACTION_VALUER_H
#define RLSNAKEGAME_ACTION_VALUER_H

#include <unordered_map>
#include <vector>

#include "hash_util.h"

template <typename S, typename A>
class ActionValuer {
 public:
  virtual ~ActionValuer() = default;
  virtual double GetValue(S, A) = 0;
  virtual void SetValue(S, A, double) = 0;
  virtual A ArgMax(S) = 0;
  virtual std::vector<S> GetStates() = 0;
  virtual std::vector<A> GetActions(S) = 0;
};

template <typename S, typename A>
class SimpleActionValuer : public ActionValuer<S, A> {
 public:
  using state_action_map = std::unordered_map<S, std::vector<A>>;

  explicit SimpleActionValuer(state_action_map state_actions,
                              double init_value = 0);
  A ArgMax(S state) override;
  std::vector<S> GetStates() override;
  std::vector<A> GetActions(S state) override;
  double GetValue(S state, A action) override;
  void SetValue(S state, A action, double new_value) override;

 private:
  using action_value_map = std::unordered_map<std::pair<S, A>, double>;
  state_action_map _state_actions;
  action_value_map _action_value_map;
  std::vector<S> _states;
};

#endif  // RLSNAKEGAME_ACTION_VALUER_H
