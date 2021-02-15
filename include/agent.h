#ifndef RLSNAKEGAME_AGENT_H
#define RLSNAKEGAME_AGENT_H

#include "learner.h"

template <typename S, typename A, typename R>
class Agent {
 public:
  virtual ~Agent() = default;
  virtual void Update(S, R) = 0;
  virtual A GetAction(S) = 0;
};

template <typename S, typename A, typename R>
class AgentImpl : public Agent<S, A, R> {
 public:
  AgentImpl() = delete;
  AgentImpl(std::shared_ptr<ActionValuer<S, A>> action_valuer,
            std::unique_ptr<Policy<S, A>> policy,
            std::unique_ptr<Learner<S, A, R>> learner, S initial_state,
            A initial_action);
  void Update(S state, R reward) override;
  A GetAction(S state) override;

 private:
  S _last_state;
  A _last_action;
  std::shared_ptr<ActionValuer<S, A>> _action_valuer;
  std::unique_ptr<Policy<S, A>> _policy;
  std::unique_ptr<Learner<S, A, R>> _learner;
};

#endif  // RLSNAKEGAME_AGENT_H
