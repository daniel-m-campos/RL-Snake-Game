#ifndef RLSNAKEGAME_ENVIRONMENT_H
#define RLSNAKEGAME_ENVIRONMENT_H

template <typename S, typename A>
class Environment {
 public:
  virtual ~Environment() = default;
  virtual void Update(A) = 0;
  virtual S GetState() = 0;
  virtual double GetReward() = 0;
};

#endif  // RLSNAKEGAME_ENVIRONMENT_H
