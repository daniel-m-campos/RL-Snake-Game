#ifndef RLSNAKEGAME_POLICY_H
#define RLSNAKEGAME_POLICY_H

template <typename S, typename A>
class Policy {
 public:
  virtual ~Policy() = default;
  virtual A operator()(S) const = 0;
  virtual double Probability(A, S) const = 0;
  virtual double Value(A, S) const = 0;
};

#endif  // RLSNAKEGAME_POLICY_H
