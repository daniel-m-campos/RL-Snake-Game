#ifndef RLSNAKEGAME_AGENT_H
#define RLSNAKEGAME_AGENT_H

#include "learner.h"

template <typename S, typename A>
class Agent {
 public:
  virtual ~Agent() = default;
  virtual void Update(S, double reward) = 0;
  virtual A GetAction(S) = 0;
};

template <typename S, typename A>
class AgentImpl : public Agent<S, A> {
 public:
  AgentImpl() = delete;
  AgentImpl(std::shared_ptr<ActionValuer<S, A>> action_valuer,
            std::unique_ptr<Policy<S, A>> policy,
            std::unique_ptr<Learner<S, A>> learner, S initial_state,
            A initial_action);
  void Update(S state, double reward) override;
  A GetAction(S state) override;

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
                           std::unique_ptr<Learner<S, A>> learner,
                           S initial_state, A initial_action)
    : _action_valuer{action_valuer},
      _policy{std::move(policy)},
      _learner{std::move(learner)},
      _last_state{initial_state},
      _last_action{initial_action} {}

template <typename S, typename A>
void AgentImpl<S, A>::Update(S state, double reward) {
  _learner->Reinforce(*_policy, *_action_valuer, _last_state, _last_action,
                      reward, state);
  _last_state = state;
}

template <typename S, typename A>
A AgentImpl<S, A>::GetAction(S state) {
  _last_action = (*_policy)(state);
  return _last_action;
}

#endif  // RLSNAKEGAME_AGENT_H
