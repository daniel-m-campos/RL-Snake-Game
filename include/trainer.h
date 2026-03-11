#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace trainer_defaults
{
int64_t constexpr num_episodes{1'000};
int64_t constexpr max_steps{1'000'000};
double constexpr epsilon{0.5};
double constexpr discount_factor{0.9};
double constexpr step_size{0.5};
} // namespace trainer_defaults

void train(std::size_t grid_width, std::size_t grid_height,
           int64_t num_episodes   = trainer_defaults::num_episodes,
           int64_t max_steps      = trainer_defaults::max_steps,
           double epsilon         = trainer_defaults::epsilon,
           double discount_factor = trainer_defaults::discount_factor,
           double step_size       = trainer_defaults::step_size);

auto file_name(std::size_t grid_width, std::size_t grid_height) -> std::string;
