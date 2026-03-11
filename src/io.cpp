#include "io.h"

#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>

#include "action_valuer_factory.h"
#include "game_environment.h"
#include "snake.h"

namespace
{

static_assert(std::endian::native == std::endian::little,
              "Binary IO requires a little-endian architecture");

std::array<char, 4> constexpr k_magic{'R', 'L', 'S', 'N'};
std::uint16_t constexpr k_version{1};

template <typename T>
void append_bytes(std::vector<std::byte> &buf, T const &value)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    auto const *ptr = reinterpret_cast<std::byte const *>(&value);
    buf.insert(buf.end(), ptr, ptr + sizeof(T));
}

template <typename T>
auto read_bytes(std::vector<std::byte> const &buf, std::size_t &offset) -> T
{
    T value{};
    std::memcpy(&value, buf.data() + offset, sizeof(T));
    offset += sizeof(T);
    return value;
}

} // namespace

void io::save(std::filesystem::path const &filepath,
              ActionValuer<GameState, snake::Direction> &action_valuer,
              bool only_argmax)
{
    StateActionMap<GameState, snake::Direction> &state_action_map =
        action_valuer.get_state_action_map();
    auto const &states = state_action_map.get_states();

    struct Entry
    {
        GameState        state;
        snake::Direction action;
        double           value;
    };

    std::vector<Entry> entries;
    entries.reserve(states.size());
    for (auto const &state : states)
    {
        if (only_argmax)
        {
            auto const action = action_valuer.arg_max(state)[0];
            entries.push_back({state, action, action_valuer.get_value(state, action)});
        }
        else
        {
            for (auto const &action : state_action_map.get_actions(state))
                entries.push_back(
                    {state, action, action_valuer.get_value(state, action)});
        }
    }

    std::vector<std::byte> buf;
    buf.reserve(12 + entries.size() * 32);

    // Header: magic (4) + version (2) + flags (2) + num_entries (4) = 12 bytes
    append_bytes(buf, k_magic);
    append_bytes(buf, k_version);
    std::uint16_t const flags{only_argmax ? std::uint16_t{1} : std::uint16_t{0}};
    append_bytes(buf, flags);
    append_bytes(buf, static_cast<std::uint32_t>(entries.size()));

    // Entries: num_points (2) + points (num_points×8) + action (1) + value (8)
    for (auto const &[state, action, value] : entries)
    {
        append_bytes(buf, static_cast<std::uint16_t>(state.body_to_food.size()));
        for (auto const &pt : state.body_to_food)
        {
            append_bytes(buf, static_cast<std::int32_t>(pt.x));
            append_bytes(buf, static_cast<std::int32_t>(pt.y));
        }
        append_bytes(buf, static_cast<std::uint8_t>(action));
        append_bytes(buf, value);
    }

    std::ofstream file{filepath, std::ios::binary | std::ios::trunc};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    file.write(reinterpret_cast<char const *>(buf.data()),
               static_cast<std::streamsize>(buf.size()));
}

auto io::load(std::filesystem::path const &filepath)
    -> std::optional<std::unique_ptr<ActionValuer<GameState, snake::Direction>>>
{
    std::ifstream file{filepath, std::ios::binary | std::ios::ate};
    if (!file.is_open())
        return std::nullopt;

    auto const file_size = static_cast<std::size_t>(file.tellg());
    file.seekg(0);

    std::vector<std::byte> buf(file_size);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    file.read(reinterpret_cast<char *>(buf.data()),
              static_cast<std::streamsize>(file_size));
    if (!file)
        return std::nullopt;

    // Minimum header: 4 (magic) + 2 (version) + 2 (flags) + 4 (num_entries) = 12
    if (file_size < 12)
        return std::nullopt;

    std::size_t offset{0};

    if (std::memcmp(buf.data(), k_magic.data(), k_magic.size()) != 0)
        return std::nullopt;
    offset += k_magic.size();

    auto const version = read_bytes<std::uint16_t>(buf, offset);
    if (version != k_version)
        return std::nullopt;
    offset += sizeof(std::uint16_t); // skip flags

    auto const num_entries = read_bytes<std::uint32_t>(buf, offset);

    std::unordered_map<GameState, std::vector<snake::Direction>> state_action_dict;
    std::unordered_map<std::pair<GameState, snake::Direction>, double> action_value_dict;
    state_action_dict.reserve(num_entries);
    action_value_dict.reserve(num_entries);

    for (std::uint32_t i{0}; i < num_entries; ++i)
    {
        if (offset + sizeof(std::uint16_t) > file_size)
            return std::nullopt;
        auto const num_points = read_bytes<std::uint16_t>(buf, offset);

        std::size_t const entry_rest =
            static_cast<std::size_t>(num_points) * (sizeof(std::int32_t) * 2) +
            sizeof(std::uint8_t) + sizeof(double);
        if (offset + entry_rest > file_size)
            return std::nullopt;

        std::vector<snake::Point<int>> body_to_food;
        body_to_food.reserve(num_points);
        for (std::uint16_t j{0}; j < num_points; ++j)
        {
            auto const x = read_bytes<std::int32_t>(buf, offset);
            auto const y = read_bytes<std::int32_t>(buf, offset);
            body_to_food.push_back({x, y});
        }

        auto const action_byte = read_bytes<std::uint8_t>(buf, offset);
        auto const value       = read_bytes<double>(buf, offset);

        GameState const        state{body_to_food};
        snake::Direction const action{static_cast<snake::Direction>(action_byte)};
        state_action_dict[state].push_back(action);
        action_value_dict[{state, action}] = value;
    }

    std::set<GameState> states;
    for (auto const &[state, actions] : state_action_dict)
        states.insert(state);

    auto action_valuer =
        ActionValuerFactory<GameState, snake::Direction>::create_simple_action_valuer(
            states, snake::directions);
    for (auto const &[state, actions] : state_action_dict)
        for (auto const &action : actions)
            action_valuer->set_value(state, action,
                                     action_value_dict.at({state, action}));

    return action_valuer;
}

auto io::find_files(std::filesystem::path const &dir) -> std::vector<std::string>
{
    std::vector<std::string> files;
    for (auto const &entry : std::filesystem::directory_iterator{dir})
    {
        if (!entry.is_regular_file())
            continue;
        auto const name = entry.path().filename().string();
        if (name.ends_with(file_suffix))
            files.push_back(name);
    }
    return files;
}

