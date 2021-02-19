#ifndef RLSNAKEGAME_ACTION_VALUER_H
#define RLSNAKEGAME_ACTION_VALUER_H

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

#endif  // RLSNAKEGAME_ACTION_VALUER_H
