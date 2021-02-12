#ifndef RLSNAKEGAME_ENVIRONMENT_H
#define RLSNAKEGAME_ENVIRONMENT_H

template <typename S, typename A, typename R>
class Environment {
 public:
  virtual ~Environment() = default;
  virtual void Update(A) = 0;
  virtual S GetState() const = 0;
  virtual R GetReward() const = 0;
};

#endif  // RLSNAKEGAME_ENVIRONMENT_H
