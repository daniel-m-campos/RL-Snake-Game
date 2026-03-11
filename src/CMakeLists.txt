add_library(snake_game STATIC
    game.cpp
    snake.cpp
    food.cpp
    game_environment.cpp
    game_simulator.cpp
    io.cpp
    trainer.cpp
)
target_include_directories(snake_game PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(snake_game PRIVATE project_warnings)

add_library(gui STATIC
    menu.cpp
    gui.cpp
)
target_include_directories(gui PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(gui
    PRIVATE
        project_warnings
        ${CURSES_LIBRARIES}
)

add_executable(RLSnakeGame
    main.cpp
    agent_controller.cpp
    keyboard_controller.cpp
    renderer.cpp
    main_utils.cpp
    game_menu_factory.cpp
)
target_include_directories(RLSnakeGame PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(RLSnakeGame
    PRIVATE
        snake_game
        gui
        project_warnings
        ${SDL2_LIBRARIES}
        ${CURSES_LIBRARIES}
)
