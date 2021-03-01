#ifndef RLSNAKEGAME_TRAINER_H
#define RLSNAKEGAME_TRAINER_H

#include <string>

void Train(std::size_t grid_width, std::size_t grid_height,
           long num_episodes = 1'000, long max_steps = 1'000'000,
           double epsilon = 0.5, double discount_factor = 0.9,
           double step_size = 0.5);

std::string FileName(std::size_t grid_width, std::size_t grid_height);

#endif  // RLSNAKEGAME_TRAINER_H
