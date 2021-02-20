#ifndef RLSNAKEGAME_LEARNER_H
#define RLSNAKEGAME_LEARNER_H

#include "policy.h"

template <typename S, typename A>
class Learner {
 public:
  virtual ~Learner() = default;
  virtual void Reinforce(const Policy<S, A>&, ActionValuer<S, A>&, S, A,
                         double reward, S) = 0;
};

template <typename S, typename A>
class QLearner : public Learner<S, A> {
 public:
  QLearner() = delete;
  QLearner(double discount_factor, double step_size);
  void Reinforce(const Policy<S, A>& policy, ActionValuer<S, A>& valuer,
                 S state, A action, double reward, S new_state) override;

 private:
  double _discount_factor;
  double _step_size;

  double NewValue(double old_value, double reward, double greedy_value) const;
};

template <typename S, typename A>
QLearner<S, A>::QLearner(double discount_factor, double step_size)
    : _discount_factor(discount_factor), _step_size(step_size) {}

template <typename S, typename A>
void QLearner<S, A>::Reinforce(const Policy<S, A>& policy,
                               ActionValuer<S, A>& valuer, S state, A action,
                               double reward, S new_state) {
  auto old_value = valuer.GetValue(state, action);
  auto new_action = valuer.ArgMax(new_state)[0];
  auto greedy_value = valuer.GetValue(new_state, new_action);
  valuer.SetValue(state, action, NewValue(old_value, reward, greedy_value));
}

template <typename S, typename A>
double QLearner<S, A>::NewValue(double old_value, double reward,
                                double greedy_value) const {
  return old_value +
         _step_size * (reward + _discount_factor * greedy_value - old_value);
}

#endif  // RLSNAKEGAME_LEARNER_H
