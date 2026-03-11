#pragma once

template <typename S, typename A> class Environment
{
  public:
    virtual ~Environment()                                    = default;
    Environment()                                             = default;
    Environment(Environment const &)                          = default;
    auto operator=(Environment const &) -> Environment &      = default;
    Environment(Environment &&)                               = default;
    auto operator=(Environment &&) -> Environment &           = default;
    virtual void update(A const &)                            = 0;
    [[nodiscard]] virtual auto get_state() const -> S const & = 0;
    [[nodiscard]] virtual auto get_reward() const -> double   = 0;
    [[nodiscard]] virtual auto has_terminated() const -> bool = 0;
};
