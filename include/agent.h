#ifndef RLSNAKEGAME_AGENT_H
#define RLSNAKEGAME_AGENT_H

template <typename S, typename A, typename R>
class Agent {
 public:
  virtual ~Agent() = default;
  virtual void Update(S, R) = 0;
  virtual A GetAction(S) const = 0;
};

#endif  // RLSNAKEGAME_AGENT_H
