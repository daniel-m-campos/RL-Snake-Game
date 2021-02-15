#ifndef RLSNAKEGAME_MOCK_POLICY_H
#define RLSNAKEGAME_MOCK_POLICY_H

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "learner.h"

template <typename S, typename A>
class MockPolicy : public Policy<S, A> {
 public:
  MOCK_METHOD(A, ParentheisesOp, (S state));
  A operator()(S state) override { return ParentheisesOp(state); }
  MOCK_METHOD(double, Probability, (A action, S given_state), (override));
};

#endif  // RLSNAKEGAME_MOCK_POLICY_H
