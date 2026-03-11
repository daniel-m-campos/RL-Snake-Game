#ifndef RLSNAKEGAME_TEST_UTILS_H
#define RLSNAKEGAME_TEST_UTILS_H

#include <iomanip>
#include <iostream>

template <typename S, typename A>
void print_value(ActionValuer<S, A> *valuer, S &state, A &action)
{
    std::cout << std::left << std::setw(4) << std::setfill(' ')
              << valuer->get_value(state, action) << "\t";
}

template <typename S, typename A>
void print(ActionValuer<S, A> *valuer, bool values, int grid_size)
{
    std::cout << std::setprecision(3);
    for (int y = grid_size - 1; y >= 0; --y)
    {
        for (int x = 0; x < grid_size; ++x)
        {
            S state{x, y};
            auto action = valuer->arg_max(state)[0];
            if (values)
            {
                print_value(valuer, state, action);
            }
            else
            {
                print_action(state, action);
            }
        }
        std::cout << std::endl;
    }
}

#endif // RLSNAKEGAME_TEST_UTILS_H
