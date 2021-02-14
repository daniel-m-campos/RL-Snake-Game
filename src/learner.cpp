#include "learner.h"

template <typename S, typename A, typename R>
QLearner<S, A, R>::QLearner(double discount_factor, double step_size)
    : _discount_factor(discount_factor), _step_size(step_size) {}

template <typename S, typename A, typename R>
void QLearner<S, A, R>::Reinforce(const Policy<S, A>& policy,
                                  ActionValuer<S, A>& valuer, S state, A action,
                                  R reward, S new_state) {
  auto old_value = valuer.GetValue(state, action);
  auto new_action = valuer.ArgMax(new_state)[0];
  auto greedy_value = valuer.GetValue(new_state, new_action);
  valuer.SetValue(state, action, NewValue(old_value, reward, greedy_value));
}

template <typename S, typename A, typename R>
double QLearner<S, A, R>::NewValue(double old_value, R reward,
                                   double greedy_value) const {
  return old_value +
         _step_size * (reward + _discount_factor * greedy_value - old_value);
}

template class QLearner<int, int, double>;