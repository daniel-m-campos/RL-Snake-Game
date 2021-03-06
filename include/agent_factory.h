#ifndef RLSNAKEGAME_AGENT_FACTORY_H
#define RLSNAKEGAME_AGENT_FACTORY_H

#include "agent.h"

template <typename S, typename A>
class AgentFactory {
 public:
  static std::unique_ptr<Agent<S, A>> CreateQAgent(
      std::unordered_map<S, std::vector<A>> state_action_map, double epsilon,
      double discount_factor, double step_size, S initial_state,
      A initial_action);

  static std::unique_ptr<Agent<S, A>> CreateQAgent(
      std::shared_ptr<ActionValuer<S, A>> action_valuer, double epsilon,
      double discount_factor, double step_size, S initial_state,
      A initial_action);
};

template <typename S, typename A>
std::unique_ptr<Agent<S, A>> AgentFactory<S, A>::CreateQAgent(
    std::unordered_map<S, std::vector<A>> state_action_map, double epsilon,
    double discount_factor, double step_size, S initial_state,
    A initial_action) {
  auto action_valuer = std::make_shared<SimpleActionValuer<S, A>>(
      std::make_unique<StateActionHashMap<S, A>>(state_action_map));
  auto policy = std::make_unique<EpsilonGreedy<S, A>>(action_valuer, epsilon);
  auto learner = std::make_unique<QLearner<S, A>>(discount_factor, step_size);
  return std::make_unique<AgentImpl<S, A>>(action_valuer, std::move(policy),
                                           std::move(learner), initial_state,
                                           initial_action);
}

template <typename S, typename A>
std::unique_ptr<Agent<S, A>> AgentFactory<S, A>::CreateQAgent(
    std::shared_ptr<ActionValuer<S, A>> action_valuer, double epsilon,
    double discount_factor, double step_size, S initial_state,
    A initial_action) {
  auto policy = std::make_unique<EpsilonGreedy<S, A>>(action_valuer, epsilon);
  auto learner = std::make_unique<QLearner<S, A>>(discount_factor, step_size);
  return std::make_unique<AgentImpl<S, A>>(action_valuer, std::move(policy),
                                           std::move(learner), initial_state,
                                           initial_action);
}

#endif  // RLSNAKEGAME_AGENT_FACTORY_H
