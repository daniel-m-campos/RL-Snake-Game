#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

struct TrainingConfig
{
    std::size_t grid_width{8};
    std::size_t grid_height{8};
    int64_t num_episodes{1'000};
    int64_t max_steps{1'000'000};
    double epsilon{0.5};
    double discount_factor{0.9};
    double step_size{0.5};
    int num_threads{1};
    int64_t checkpoint_interval{0}; // episodes between checkpoints; 0 = disabled
};

struct TrainingProgress
{
    std::atomic<int64_t> episodes_completed{0};
    std::atomic<int64_t> total_episodes{0};
    std::atomic<bool> cancel_requested{false};
    std::atomic<bool> training_complete{false};
    std::atomic<bool> merging{false};
    std::atomic<bool> saving{false};
    std::atomic<int64_t> qtable_size{0};
    std::atomic<int64_t> checkpoints_completed{0};
};
