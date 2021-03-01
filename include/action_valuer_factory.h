#ifndef RLSNAKEGAME_ACTION_VALUER_FACTORY_H
#define RLSNAKEGAME_ACTION_VALUER_FACTORY_H

#include "action_valuer.h"

template <typename S, typename A>
class ActionValuerFactory {
 public:
  static std::unique_ptr<ActionValuer<S, A>> CreateSimpleActionValuer(
      const std::set<S>& states, const std::set<A>& actions);
};

template <typename S, typename A>
std::unique_ptr<ActionValuer<S, A>>
ActionValuerFactory<S, A>::CreateSimpleActionValuer(
    const std::set<S>& states, const std::set<A>& actions) {
  return std::make_unique<SimpleActionValuer<S, A>>(
      std::make_unique<SimpleStateActionMap<S, A>>(states, actions));
}

#endif  // RLSNAKEGAME_ACTION_VALUER_FACTORY_H
