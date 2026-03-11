#pragma once
#include "action_valuer.h"

template <typename S, typename A> class ActionValuerFactory
{
  public:
    static auto create_simple_action_valuer(std::set<S> const &states,
                                            std::set<A> const &actions)
        -> std::unique_ptr<ActionValuer<S, A>>;
};

template <typename S, typename A>
auto ActionValuerFactory<S, A>::create_simple_action_valuer(std::set<S> const &states,
                                                            std::set<A> const &actions)
    -> std::unique_ptr<ActionValuer<S, A>>
{
    return std::make_unique<SimpleActionValuer<S, A>>(
        std::make_unique<SimpleStateActionMap<S, A>>(states, actions));
}
