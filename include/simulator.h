#pragma once

#include <cstdint>

#include "agent.h"
#include "environment.h"

template <typename S, typename A>
static void run_episode(Environment<S, A> &environment, Agent<S, A> &agent,
                        int64_t max_steps)
{
    for (int64_t count = 0; !environment.has_terminated() && count < max_steps; ++count)
    {
        auto action = agent.get_action(environment.get_state());
        environment.update(action);
        agent.update(environment.get_state(), environment.get_reward());
    }
}

class Simulator
{
  public:
    virtual ~Simulator()                                           = default;
    Simulator()                                                    = default;
    Simulator(Simulator const &)                                   = default;
    auto operator=(Simulator const &) -> Simulator &               = default;
    Simulator(Simulator &&)                                        = default;
    auto operator=(Simulator &&) -> Simulator &                    = default;
    virtual void simulate(int64_t num_episodes, int64_t max_steps) = 0;
};
