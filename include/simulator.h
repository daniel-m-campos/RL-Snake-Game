#ifndef RLSNAKEGAME_SIMULATOR_H
#define RLSNAKEGAME_SIMULATOR_H

#include <utility>

#include "agent.h"
#include "environment.h"

template <typename S, typename A>
static void RunEpisode(Environment<S, A>& environment, Agent<S, A>& agent,
                       long max_steps) {
  for (int count = 0; !environment.HasTerminated() && count < max_steps;
       ++count) {
    auto position = environment.GetState();
    auto action = agent.GetAction(position);
    environment.Update(action);
    auto reward = environment.GetReward();
    agent.Update(environment.GetState(), reward);
  }
}

class Simulator {
 public:
  virtual ~Simulator() = default;
  virtual void Simulate(long num_episodes, long max_steps) = 0;
};

#endif  // RLSNAKEGAME_SIMULATOR_H
