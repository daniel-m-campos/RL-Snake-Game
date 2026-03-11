#include "io.h"

#include <filesystem>
#include <fstream>
#include <set>

#include "action_valuer.h"
#include "action_valuer_factory.h"
#include "game_environment.h"
#include "gtest/gtest.h"
#include "snake.h"

namespace
{

auto make_test_action_valuer()
    -> std::unique_ptr<ActionValuer<GameState, snake::Direction>>
{
    std::set<GameState> states{GameState{{{0, 1}}}, GameState{{{-15, -15}}}};
    auto av = ActionValuerFactory<GameState, snake::Direction>::create_simple_action_valuer(
        states, snake::directions);
    av->set_value(GameState{{{0, 1}}}, snake::Direction::kUp, 0.5);
    av->set_value(GameState{{{-15, -15}}}, snake::Direction::kDown, 0.475653);
    return av;
}

} // namespace

class IOFixture : public ::testing::Test
{
  protected:
    std::filesystem::path tmp_file{
        std::filesystem::temp_directory_path() / "test_action_valuer.bin"};
    void TearDown() override { std::filesystem::remove(tmp_file); }
};

TEST_F(IOFixture, TestRoundTrip)
{
    auto original = make_test_action_valuer();
    io::save(tmp_file, *original, /*only_argmax=*/false);

    auto result = io::load(tmp_file);
    ASSERT_TRUE(result.has_value());
    auto &loaded = *result.value();

    EXPECT_DOUBLE_EQ(loaded.get_value(GameState{{{0, 1}}}, snake::Direction::kUp), 0.5);
    EXPECT_DOUBLE_EQ(
        loaded.get_value(GameState{{{-15, -15}}}, snake::Direction::kDown), 0.475653);
    EXPECT_DOUBLE_EQ(loaded.get_value(GameState{{{0, 1}}}, snake::Direction::kDown), 0.0);
}

TEST_F(IOFixture, TestLoadMissingFile)
{
    auto result = io::load("/nonexistent/path/file.bin");
    EXPECT_FALSE(result.has_value());
}

TEST_F(IOFixture, TestLoadBadMagic)
{
    {
        std::ofstream f{tmp_file, std::ios::binary};
        f << "BADMAGIC";
    }
    auto result = io::load(tmp_file);
    EXPECT_FALSE(result.has_value());
}

TEST_F(IOFixture, TestLoadTruncatedFile)
{
    {
        std::ofstream f{tmp_file, std::ios::binary};
        f << "RLS"; // only 3 bytes — truncated before full header
    }
    auto result = io::load(tmp_file);
    EXPECT_FALSE(result.has_value());
}

TEST(IOFindFilesTest, FindsBinNotTxt)
{
    auto const tmpdir =
        std::filesystem::temp_directory_path() / "io_test_find_files";
    std::filesystem::create_directories(tmpdir);

    {std::ofstream{tmpdir / "8x8_action_valuer.bin"} << "placeholder";}
    {std::ofstream{tmpdir / "8x8_action_valuer.txt"} << "old format";}

    auto files = io::find_files(tmpdir);
    EXPECT_EQ(files.size(), 1u);
    EXPECT_EQ(files[0], "8x8_action_valuer.bin");

    std::filesystem::remove_all(tmpdir);
}
