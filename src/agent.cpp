#include "agent.h"

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

template class AgentImpl<int, int>;