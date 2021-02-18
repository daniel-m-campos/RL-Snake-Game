add_executable(
        INTEGRATION_TEST
        gridworld.cpp
        gridworld_test.cpp
)

target_link_libraries(
        INTEGRATION_TEST
        gtest
        gtest_main
        pthread
)
