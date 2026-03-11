#ifndef RLSNAKEGAME_MOCK_ACTION_VALUER_H
#define RLSNAKEGAME_MOCK_ACTION_VALUER_H

#include "action_valuer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

template <typename S, typename A> class MockActionValuer : public ActionValuer<S, A>
{
  public:
    MOCK_METHOD(double, get_value, (S state, A action), (override));
    MOCK_METHOD(void, set_value, (S state, A action, double value), (override));
    MOCK_METHOD(std::vector<A>, arg_max, (S state), (override));
    MOCK_METHOD((StateActionMap<S, A> &), get_state_action_map, (), (override));
};

#endif // RLSNAKEGAME_MOCK_ACTION_VALUER_H
