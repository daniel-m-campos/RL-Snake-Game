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
    auto action = agent.GetAction(environment.GetState());
    environment.Update(action);
    agent.Update(environment.GetState(), environment.GetReward());
  }
}

class Simulator {
 public:
  virtual ~Simulator() = default;
  virtual void Simulate(long num_episodes, long max_steps) = 0;
};

#endif  // RLSNAKEGAME_SIMULATOR_H
