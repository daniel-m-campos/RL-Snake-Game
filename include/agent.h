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

#endif  // RLSNAKEGAME_AGENT_H
