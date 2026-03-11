#pragma once

#include "gmock/gmock.h"
#include <policy.h>

template <typename S, typename A> class MockPolicy : public Policy<S, A>
{
  public:
    MOCK_METHOD(A, parentheses_op, (S state));
    A operator()(S state) override
    {
        return parentheses_op(state);
    }
    MOCK_METHOD(double, probability, (A action, S given_state), (override));
};
