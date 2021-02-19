add_executable(
        INTEGRATION_TEST
        test.cpp
        gridworld.cpp
        gridworld_test.cpp
)

target_link_libraries(
        INTEGRATION_TEST
        snake_game
        gtest
        gtest_main
        pthread
)
