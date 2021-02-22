add_executable(
        UNIT_TEST
        test.cpp
        snake_tests.cpp
        policy_tests.cpp
        action_valuer_tests.cpp
        learner_tests.cpp
        agent_tests.cpp
        agent_factory_tests.cpp
        food_tests.cpp
        game_environment_test.cpp
        state_action_map_test.cpp
        io_test.cpp
)

target_link_libraries(
        UNIT_TEST
        snake_game
        gtest
        gmock
        gtest_main
        pthread
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/16x16_action_valuer.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)