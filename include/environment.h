#pragma once

template <typename S, typename A> class Environment
{
  public:
    virtual ~Environment()                               = default;
    Environment()                                        = default;
    Environment(Environment const &)                     = default;
    auto operator=(Environment const &) -> Environment & = default;
    Environment(Environment &&)                          = default;
    auto operator=(Environment &&) -> Environment &      = default;
    virtual void update(A const &)                       = 0;
    virtual auto get_state() -> S const &                = 0;
    virtual auto get_reward() -> double                  = 0;
    virtual auto has_terminated() -> bool                = 0;
};
