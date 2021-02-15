#ifndef RLSNAKEGAME_MOCK_ACTION_VALUER_H
#define RLSNAKEGAME_MOCK_ACTION_VALUER_H

#include "action_valuer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

template <typename S, typename A>
class MockActionValuer : public ActionValuer<S, A> {
 public:
  MOCK_METHOD(double, GetValue, (S state, A action), (override));
  MOCK_METHOD(void, SetValue, (S state, A action, double value), (override));
  MOCK_METHOD(std::vector<A>, ArgMax, (S state), (override));
  MOCK_METHOD(std::vector<S>, GetStates, (), (override));
  MOCK_METHOD(std::vector<A>, GetActions, (S state), (override));
};

#endif  // RLSNAKEGAME_MOCK_ACTION_VALUER_H
