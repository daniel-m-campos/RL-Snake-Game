#include "agent_factory.h"
#include "gtest/gtest.h"

TEST(RLFactoryTest, TestCreateQAgent)
{
    double constexpr test_epsilon{0.1};
    double constexpr test_discount_factor{0.95};
    double constexpr test_step_size{0.2};
    std::unordered_map<int, std::vector<int>> state_action_map{
        {0, {0, 1}},
        {1, {1, 3}},
        {2, {2, 4}},
    };
    auto agent = AgentFactory<int, int>::create_q_agent(
        state_action_map, test_epsilon, test_discount_factor, test_step_size, 0, 0);
}
