#pragma once

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "hash_util.h" // IWYU pragma: keep
#include "state_action_map.h"

template <typename S, typename A> class ActionValuer
{
  public:
    virtual ~ActionValuer()                                       = default;
    ActionValuer()                                                = default;
    ActionValuer(ActionValuer const &)                            = default;
    ActionValuer &operator=(ActionValuer const &)                 = default;
    ActionValuer(ActionValuer &&)                                 = default;
    ActionValuer &operator=(ActionValuer &&)                      = default;
    virtual auto get_value(S, A) -> double                        = 0;
    virtual void set_value(S, A, double)                          = 0;
    virtual auto arg_max(S) -> std::vector<A>                     = 0;
    virtual auto get_state_action_map() -> StateActionMap<S, A> & = 0;
};

template <typename S, typename A> class SimpleActionValuer : public ActionValuer<S, A>
{
  public:
    using action_value_map = std::unordered_map<std::pair<S, A>, double>;

    SimpleActionValuer() = delete;
    explicit SimpleActionValuer(std::unique_ptr<StateActionMap<S, A>> state_action_map,
                                action_value_map action_value_map);
    explicit SimpleActionValuer(std::unique_ptr<StateActionMap<S, A>> state_action_map,
                                double init_value = 0);
    auto arg_max(S state) -> std::vector<A> override;
    auto get_value(S state, A action) -> double override;
    void set_value(S state, A action, double new_value) override;
    auto get_state_action_map() -> StateActionMap<S, A> & override;

  private:
    std::unique_ptr<StateActionMap<S, A>> _state_action_map;
    action_value_map _action_value_map;
};

template <typename S, typename A>
SimpleActionValuer<S, A>::SimpleActionValuer(
    std::unique_ptr<StateActionMap<S, A>> state_action_map, double init_value)
    : _state_action_map{std::move(state_action_map)}
{
    for (auto const &state : _state_action_map->get_states())
    {
        for (auto const &action : _state_action_map->get_actions(state))
        {
            _action_value_map[{state, action}] = init_value;
        }
    }
}

template <typename S, typename A>
SimpleActionValuer<S, A>::SimpleActionValuer(
    std::unique_ptr<StateActionMap<S, A>> state_action_map,
    SimpleActionValuer::action_value_map action_value_map)
    : _state_action_map{std::move(state_action_map)},
      _action_value_map{action_value_map}
{
}

template <typename S, typename A>
std::vector<A> SimpleActionValuer<S, A>::arg_max(S state)
{
    auto compare = [&](auto const &left, auto const &right)
    { return _action_value_map[{state, left}] > _action_value_map[{state, right}]; };
    auto actions = _state_action_map->get_actions(state);
    std::sort(actions.begin(), actions.end(), compare);
    auto const max_value = _action_value_map[{state, actions[0]}];
    auto iter            = actions.begin();
    while (iter != actions.end())
    {
        if (_action_value_map[{state, *iter}] != max_value)
        {
            iter = actions.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
    return actions;
}

template <typename S, typename A>
double SimpleActionValuer<S, A>::get_value(S state, A action)
{
    return _action_value_map[{state, action}];
}

template <typename S, typename A>
void SimpleActionValuer<S, A>::set_value(S state, A action, double new_value)
{
    _action_value_map[{state, action}] = new_value;
}

template <typename S, typename A>
auto SimpleActionValuer<S, A>::get_state_action_map() -> StateActionMap<S, A> &
{
    return *_state_action_map;
}
