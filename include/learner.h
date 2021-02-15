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

#endif  // RLSNAKEGAME_LEARNER_H
