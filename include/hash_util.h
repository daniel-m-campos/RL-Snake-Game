#pragma once

#include <utility>

template <class T> void hash_combine(std::size_t &seed, T const &v)
{
    std::size_t constexpr golden_ratio_hash{0x9e3779b9};
    int constexpr hash_shift_left{6};
    int constexpr hash_shift_right{2};
    std::hash<T> hasher;
    seed ^= hasher(v) + golden_ratio_hash + (seed << hash_shift_left) +
            (seed >> hash_shift_right);
}

template <typename S, typename T> struct std::hash<std::pair<S, T>>
{
    size_t operator()(pair<S, T> const &v) const
    {
        size_t seed = 0;
        ::hash_combine(seed, v.first);
        ::hash_combine(seed, v.second);
        return seed;
    }
};
