#pragma once

#include <algorithm>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

#include "action_valuer.h"
#include "state_action_map.h"

template <typename S, typename A> class Policy
{
  public:
    virtual ~Policy()                          = default;
    Policy()                                   = default;
    Policy(Policy const &)                     = default;
    auto operator=(Policy const &) -> Policy & = default;
    Policy(Policy &&)                          = default;
    auto operator=(Policy &&) -> Policy &      = default;
    virtual auto operator()(S) -> A            = 0;
    virtual auto probability(A, S) -> double   = 0;
};

template <typename S, typename A> class EpsilonGreedy : public Policy<S, A>
{
  public:
    using state_action_map = std::unordered_map<S, std::vector<A>>;

    EpsilonGreedy() = delete;
    EpsilonGreedy(std::shared_ptr<ActionValuer<S, A>> _action_valuer, double epsilon);
    EpsilonGreedy(state_action_map state_actions, double epsilon);
    auto operator()(S state) -> A override;
    auto probability(A action, S given_state) -> double override;

  private:
    double _epsilon;
    std::shared_ptr<ActionValuer<S, A>> _action_valuer;
    std::mt19937_64 _engine;
    std::uniform_real_distribution<> _uniform_dist;
};

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(std::shared_ptr<ActionValuer<S, A>> action_valuer,
                                   double epsilon)
    : _epsilon(epsilon), _action_valuer(action_valuer)
{
}

template <typename S, typename A>
EpsilonGreedy<S, A>::EpsilonGreedy(EpsilonGreedy::state_action_map state_actions,
                                   double epsilon)
    : _epsilon(epsilon),
      _action_valuer(std::make_shared<SimpleActionValuer<S, A>>(
          std::make_unique<StateActionHashMap<S, A>>(state_actions))),
      _uniform_dist(std::uniform_real_distribution<>(0, 1)),
      _engine(std::random_device()())
{
}

template <typename S, typename A>
auto EpsilonGreedy<S, A>::probability(A action, S state) -> double
{
    auto actions = _action_valuer->arg_max(state);
    double num_actions =
        _action_valuer->get_state_action_map().get_actions(state).size();
    if (std::find(actions.begin(), actions.end(), action) != actions.end())
    {
        return (_epsilon / num_actions) + ((1 - _epsilon) / actions.size());
    }
    return _epsilon / num_actions;
}

template <typename S, typename A> auto EpsilonGreedy<S, A>::operator()(S state) -> A
{
    std::vector<A> actions;
    if (_uniform_dist(_engine) < _epsilon)
    {
        actions = _action_valuer->get_state_action_map().get_actions(state);
    }
    else
    {
        actions = _action_valuer->arg_max(state);
    }
    std::shuffle(actions.begin(), actions.end(), _engine);
    return actions[0];
}
