#ifndef RLSNAKEGAME_RL_FACTORY_H
#define RLSNAKEGAME_RL_FACTORY_H

#include "agent.h"

template <typename S, typename A, typename R>
class RLFactory {
 public:
  static std::unique_ptr<Agent<S, A, R>> create_q_agent(
      std::unordered_map<S, std::vector<A>> state_action_map, double epsilon,
      double discount_factor, double step_size, S initial_state,
      A initial_action);
};

#endif  // RLSNAKEGAME_RL_FACTORY_H
