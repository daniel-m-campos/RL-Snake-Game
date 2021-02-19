#ifndef RLSNAKEGAME_TEST_UTILS_H
#define RLSNAKEGAME_TEST_UTILS_H

#include <iomanip>
#include <iostream>

template <typename S, typename A>
void PrintValue(ActionValuer<S, A>* valuer, S& state, A& action) {
  std::cout << std::left << std::setw(4) << std::setfill(' ')
            << valuer->GetValue(state, action) << "\t";
}

template <typename S, typename A>
void Print(ActionValuer<S, A>* valuer, bool values, int grid_size) {
  std::cout << std::setprecision(3);
  for (int y = grid_size - 1; y >= 0; --y) {
    for (int x = 0; x < grid_size; ++x) {
      S state{x, y};
      auto action = valuer->ArgMax(state)[0];
      if (values) {
        PrintValue(valuer, state, action);
      } else {
        PrintAction(state, action);
      }
    }
    std::cout << std::endl;
  }
}

#endif  // RLSNAKEGAME_TEST_UTILS_H
