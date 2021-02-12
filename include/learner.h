#ifndef RLSNAKEGAME_LEARNER_H
#define RLSNAKEGAME_LEARNER_H

#include "policy.h"

template <typename S, typename A, typename R>
class Learner {
 public:
  virtual ~Learner() = default;
  virtual Policy Reinforce(Policy, S, A, R, S, A) const = 0;
};

#endif  // RLSNAKEGAME_LEARNER_H
