#include "rl_factory.h"

template <typename S, typename A, typename R>
std::unique_ptr<Agent<S, A, R>> RLFactory<S, A, R>::create_q_agent(
    std::unordered_map<S, std::vector<A>> state_action_map, double epsilon,
    double discount_factor, double step_size, S initial_state,
    A initial_action) {
  auto action_valuer =
      std::make_shared<SimpleActionValuer<S, A>>(state_action_map);
  auto policy = std::make_unique<EpsilonGreedy<S, A>>(action_valuer, epsilon);
  auto learner =
      std::make_unique<QLearner<S, A, R>>(discount_factor, step_size);
  return std::make_unique<AgentImpl<S, A, R>>(action_valuer, std::move(policy),
                                              std::move(learner), initial_state,
                                              initial_action);
}

template class RLFactory<int, int, double>;