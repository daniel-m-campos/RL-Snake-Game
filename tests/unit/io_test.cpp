#include "io.h"

#include "action_valuer.h"
#include "gtest/gtest.h"
#include "state_action_map.h"

class IOFixture : public ::testing::Test {
 public:
  std::string filename{"16x16_action_valuer.txt"};
};

TEST_F(IOFixture, TestLoad) {
  auto action_valuer = io::Load<GameState, snake::Direction>(filename);
  EXPECT_EQ(action_valuer->GetValue({0, 0, 0, 0}, snake::Direction{0}), 0.367);
  EXPECT_EQ(action_valuer->GetValue({-2, 2, 0, 0}, snake::Direction{3}),
            0.02153);
}