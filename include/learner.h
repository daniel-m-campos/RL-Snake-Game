#pragma once

#include "action_valuer.h"
#include "policy.h"

template <typename S, typename A> class Learner
{
  public:
    virtual ~Learner()                           = default;
    Learner()                                    = default;
    Learner(Learner const &)                     = default;
    auto operator=(Learner const &) -> Learner & = default;
    Learner(Learner &&)                          = default;
    auto operator=(Learner &&) -> Learner &      = default;
    virtual void reinforce(Policy<S, A> const &, ActionValuer<S, A> &, S, A,
                           double reward, S)     = 0;
};

template <typename S, typename A> class QLearner : public Learner<S, A>
{
  public:
    QLearner() = delete;
    QLearner(double discount_factor, double step_size);
    void reinforce(Policy<S, A> const &policy, ActionValuer<S, A> &valuer, S state,
                   A action, double reward, S new_state) override;

  private:
    double _discount_factor;
    double _step_size;

    auto new_value(double old_value, double reward, double greedy_value) const
        -> double;
};

template <typename S, typename A>
QLearner<S, A>::QLearner(double discount_factor, double step_size)
    : _discount_factor(discount_factor), _step_size(step_size)
{
}

template <typename S, typename A>
void QLearner<S, A>::reinforce(Policy<S, A> const & /*policy*/,
                               ActionValuer<S, A> &valuer, S state, A action,
                               double reward, S new_state)
{
    auto old_value    = valuer.get_value(state, action);
    auto new_action   = valuer.arg_max(new_state)[0];
    auto greedy_value = valuer.get_value(new_state, new_action);
    valuer.set_value(state, action, new_value(old_value, reward, greedy_value));
}

template <typename S, typename A>
auto QLearner<S, A>::new_value(double old_value, double reward,
                               double greedy_value) const -> double
{
    return old_value +
           (_step_size * (reward + (_discount_factor * greedy_value) - old_value));
}
