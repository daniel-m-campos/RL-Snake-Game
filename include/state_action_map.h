#pragma once
#include <algorithm>
#include <set>
#include <span>
#include <unordered_map>
#include <vector>

template <typename S, typename A> class StateActionMap
{
  public:
    StateActionMap()                                           = default;
    StateActionMap(StateActionMap const &)                     = default;
    StateActionMap(StateActionMap &&)                          = default;
    auto operator=(StateActionMap const &) -> StateActionMap & = default;
    auto operator=(StateActionMap &&) -> StateActionMap &      = default;
    virtual ~StateActionMap()                                  = default;
    virtual std::vector<S> const &get_states()                 = 0;
    virtual std::vector<A> const &get_actions(S)               = 0;
};

template <typename S, typename A> class StateActionHashMap : public StateActionMap<S, A>
{
  public:
    using state_action_map = std::unordered_map<S, std::vector<A>>;
    StateActionHashMap()   = delete;
    explicit StateActionHashMap(state_action_map sam);
    auto get_states() -> std::vector<S> const & override;
    auto get_actions(S state) -> std::vector<A> const & override;

  private:
    state_action_map _state_actions;
    std::vector<S> _states;
};

template <typename S, typename A>
class SimpleStateActionMap : public StateActionMap<S, A>
{
  public:
    SimpleStateActionMap() = delete;
    explicit SimpleStateActionMap(std::set<S> states, std::span<A const> actions);
    auto get_states() -> std::vector<S> const & override;
    auto get_actions(S state) -> std::vector<A> const & override;

  private:
    std::set<S> _states;
    std::vector<S> _unique_states;
    std::vector<A> _unique_actions;
};

template <typename S, typename A>
StateActionHashMap<S, A>::StateActionHashMap(StateActionHashMap::state_action_map sam)
    : _state_actions{sam}
{
}

template <typename S, typename A>
auto StateActionHashMap<S, A>::get_states() -> std::vector<S> const &
{
    if (_states.empty())
    {
        for (auto const &[state, actions] : _state_actions)
        {
            _states.push_back(state);
        }
        std::sort(_states.begin(), _states.end());
    }
    return _states;
}

template <typename S, typename A>
std::vector<A> const &StateActionHashMap<S, A>::get_actions(S state)
{
    auto const it = _state_actions.find(state);
    if (it == _state_actions.end())
    {
        static std::vector<A> const empty_actions{};
        return empty_actions;
    }
    return it->second;
}

template <typename S, typename A>
SimpleStateActionMap<S, A>::SimpleStateActionMap(std::set<S> states,
                                                 std::span<A const> actions)
    : _states{std::move(states)}, _unique_actions{actions.begin(), actions.end()}
{
}

template <typename S, typename A>
std::vector<A> const &SimpleStateActionMap<S, A>::get_actions(S state)
{
    _states.insert(state);
    return _unique_actions;
}

template <typename S, typename A>
std::vector<S> const &SimpleStateActionMap<S, A>::get_states()
{
    _unique_states.clear();
    std::copy(_states.begin(), _states.end(), std::back_inserter(_unique_states));
    return _unique_states;
}
