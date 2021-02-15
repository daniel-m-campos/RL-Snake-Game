#ifndef RLSNAKEGAME_POLICY_H
#define RLSNAKEGAME_POLICY_H

#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

#include "action_valuer.h"
#include "hash_util.h"

template <typename S, typename A>
class Policy {
 public:
  virtual ~Policy() = default;
  virtual A operator()(S) = 0;
  virtual double Probability(A, S) = 0;
};

template <typename S, typename A>
class EpsilonGreedy : public Policy<S, A> {
 public:
  using state_action_map = std::unordered_map<S, std::vector<A>>;

  EpsilonGreedy() = delete;
  EpsilonGreedy(std::shared_ptr<ActionValuer<S, A>> _action_valuer,
                double epsilon);
  EpsilonGreedy(state_action_map state_actions, double epsilon);
  A operator()(S state) override;
  double Probability(A action, S given_state) override;

 private:
  double _epsilon;
  std::shared_ptr<ActionValuer<S, A>> _action_valuer;
  std::mt19937_64 _engine;
  std::uniform_real_distribution<> _uniform_dist;
};

#endif  // RLSNAKEGAME_POLICY_H
