#pragma once

#include "learner.h"
#include <memory>

template <typename S, typename A> class Agent
{
  public:
    virtual ~Agent()                         = default;
    Agent()                                  = default;
    Agent(Agent const &)                     = default;
    auto operator=(Agent const &) -> Agent & = default;
    Agent(Agent &&)                          = default;
    auto operator=(Agent &&) -> Agent &      = default;
    virtual void update(S, double reward)    = 0;
    virtual auto get_action(S) -> A          = 0;
};

template <typename S, typename A> class AgentImpl : public Agent<S, A>
{
  public:
    AgentImpl() = delete;
    AgentImpl(std::shared_ptr<ActionValuer<S, A>> action_valuer,
              std::unique_ptr<Policy<S, A>> policy,
              std::unique_ptr<Learner<S, A>> learner, S initial_state,
              A initial_action);
    void update(S state, double reward) override;
    auto get_action(S state) -> A override;

  private:
    S _last_state;
    A _last_action;
    std::shared_ptr<ActionValuer<S, A>> _action_valuer;
    std::unique_ptr<Policy<S, A>> _policy;
    std::unique_ptr<Learner<S, A>> _learner;
};

template <typename S, typename A>
AgentImpl<S, A>::AgentImpl(std::shared_ptr<ActionValuer<S, A>> action_valuer,
                           std::unique_ptr<Policy<S, A>> policy,
                           std::unique_ptr<Learner<S, A>> learner, S initial_state,
                           A initial_action)
    : _action_valuer{action_valuer}, _policy{std::move(policy)},
      _learner{std::move(learner)}, _last_state{initial_state},
      _last_action{initial_action}
{
}

template <typename S, typename A> void AgentImpl<S, A>::update(S state, double reward)
{
    _learner->reinforce(*_policy, *_action_valuer, _last_state, _last_action, reward,
                        state);
    _last_state = state;
}

template <typename S, typename A> auto AgentImpl<S, A>::get_action(S state) -> A
{
    _last_action = (*_policy)(state);
    return _last_action;
}
