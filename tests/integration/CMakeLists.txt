add_executable(
        INTEGRATION_TEST
        test.cpp
        gridworld.cpp
        gridworld_test.cpp
        game_environment_test.cpp
)

target_link_libraries(
        INTEGRATION_TEST
        snake_game
        gtest
        gtest_main
        pthread
)

add_executable(
        GUI_TEST
        gui_test.cpp
)

target_link_libraries(GUI_TEST ${CURSES_LIBRARIES} gui)