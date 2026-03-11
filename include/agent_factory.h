#pragma once

#include "agent.h"

template <typename S, typename A> class AgentFactory
{
  public:
    static auto
    create_q_agent(std::unordered_map<S, std::vector<A>> state_action_map,
                   double epsilon, double discount_factor, double step_size,
                   S initial_state,
                   A initial_action) // NOLINT(bugprone-easily-swappable-parameters)
        -> std::unique_ptr<Agent<S, A>>;

    static auto
    create_q_agent(std::shared_ptr<ActionValuer<S, A>> action_valuer, double epsilon,
                   double discount_factor, double step_size, S initial_state,
                   A initial_action) // NOLINT(bugprone-easily-swappable-parameters)
        -> std::unique_ptr<Agent<S, A>>;
};

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
template <typename S, typename A>
auto AgentFactory<S, A>::create_q_agent(
    std::unordered_map<S, std::vector<A>> state_action_map, double epsilon,
    double discount_factor, double step_size, S initial_state, A initial_action)
    -> std::unique_ptr<Agent<S, A>>
{
    auto action_valuer = std::make_shared<SimpleActionValuer<S, A>>(
        std::make_unique<StateActionHashMap<S, A>>(state_action_map));
    auto policy  = std::make_unique<EpsilonGreedy<S, A>>(action_valuer, epsilon);
    auto learner = std::make_unique<QLearner<S, A>>(discount_factor, step_size);
    return std::make_unique<AgentImpl<S, A>>(action_valuer, std::move(policy),
                                             std::move(learner), initial_state,
                                             initial_action);
}
// NOLINTEND(bugprone-easily-swappable-parameters)

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
template <typename S, typename A>
auto AgentFactory<S, A>::create_q_agent(
    std::shared_ptr<ActionValuer<S, A>> action_valuer, double epsilon,
    double discount_factor, double step_size, S initial_state, A initial_action)
    -> std::unique_ptr<Agent<S, A>>
{
    auto policy  = std::make_unique<EpsilonGreedy<S, A>>(action_valuer, epsilon);
    auto learner = std::make_unique<QLearner<S, A>>(discount_factor, step_size);
    return std::make_unique<AgentImpl<S, A>>(action_valuer, std::move(policy),
                                             std::move(learner), initial_state,
                                             initial_action);
}
// NOLINTEND(bugprone-easily-swappable-parameters)
