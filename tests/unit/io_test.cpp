#include "io.h"

#include "action_valuer.h"
#include "state_action_map.h"
#include "gtest/gtest.h"

class IOFixture : public ::testing::Test
{
  public:
    std::string filename{"16x16_action_valuer.txt"};
};

TEST_F(IOFixture, TestLoad)
{
    auto action_valuer = io::load<GameState, snake::Direction>(filename);
    EXPECT_EQ(action_valuer->get_value({{{-15, -15}}}, snake::Direction{0}), 0.0);
    EXPECT_EQ(action_valuer->get_value({{{-15, -15}}}, snake::Direction{1}), 0.475653);
}

TEST_F(IOFixture, TestFindFiles)
{
    auto files = io::find_files();
    EXPECT_EQ(files.size(), 1);
    EXPECT_EQ(files[0], filename);
}
