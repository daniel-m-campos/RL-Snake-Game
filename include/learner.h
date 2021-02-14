#ifndef RLSNAKEGAME_LEARNER_H
#define RLSNAKEGAME_LEARNER_H

#include "policy.h"

template <typename S, typename A, typename R>
class Learner {
 public:
  virtual ~Learner() = default;
  virtual void Reinforce(Policy<S, A>&, S, A, R, S, A) = 0;
};

template <typename S, typename A, typename R>
class QLearner : public Learner<S, A, R> {
 public:
  QLearner(double discount_factor, double step_size);

 private:
  double _discount_factor;
  double _step_size;
};

#endif  // RLSNAKEGAME_LEARNER_H
