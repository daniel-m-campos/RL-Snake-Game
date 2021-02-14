#include "learner.h"

template <typename S, typename A, typename R>
QLearner<S, A, R>::QLearner(double discount_factor, double step_size)
    : _discount_factor(discount_factor), _step_size(step_size) {}