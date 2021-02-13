#ifndef RLSNAKEGAME_POLICY_H
#define RLSNAKEGAME_POLICY_H

#include <random>
#include <unordered_map>
#include <vector>

#include "hash_util.h"

template <typename S, typename A>
class Policy {
 public:
  virtual ~Policy() = default;
  virtual A operator()(S) = 0;
  virtual double Probability(A, S) = 0;
  virtual double Value(S, A) = 0;
  virtual size_t StateSize() const = 0;
  virtual size_t ActionSize() const = 0;
};

template <typename S, typename A>
class EpsilonGreedy : public Policy<S, A> {
 public:
  using state_action_map = std::unordered_map<S, std::vector<A>>;
  using action_value_map = std::unordered_map<std::pair<S, A>, double>;
  EpsilonGreedy(size_t num_states, size_t num_actions, double epsilon);
  EpsilonGreedy(state_action_map state_actions, double epsilon);
  EpsilonGreedy(state_action_map state_actions, action_value_map action_values,
                double epsilon);
  A operator()(S state) override;
  double Probability(A action, S given_state) override;
  double Value(S state, A action) override;
  size_t StateSize() const override;
  size_t ActionSize() const override;

 private:
  double _epsilon;
  size_t _num_states;
  size_t _num_actions;
  state_action_map _state_actions;
  std::unordered_map<S, bool> _actions;  // this is hack to get a unique list..
  action_value_map _action_values;
  std::mt19937_64 _engine;
  std::uniform_real_distribution<> _uniform_dist;

  std::vector<A> GreedyAction(S state);
  void CacheActions();
  std::vector<A> GetActions(const S& state);
};

#endif  // RLSNAKEGAME_POLICY_H
