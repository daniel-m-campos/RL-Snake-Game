add_executable(
        INTEGRATION_TEST
        grid_world.cpp
        test.cpp
)

target_link_libraries(
        INTEGRATION_TEST
        gtest
        gtest_main
        pthread
)
