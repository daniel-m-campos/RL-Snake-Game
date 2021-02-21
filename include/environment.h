#ifndef RLSNAKEGAME_ENVIRONMENT_H
#define RLSNAKEGAME_ENVIRONMENT_H

template <typename S, typename A>
class Environment {
 public:
  virtual ~Environment() = default;
  virtual void Update(const A&) = 0;
  virtual const S& GetState() = 0;
  virtual double GetReward() = 0;
  virtual bool HasTerminated() = 0;
};

#endif  // RLSNAKEGAME_ENVIRONMENT_H
