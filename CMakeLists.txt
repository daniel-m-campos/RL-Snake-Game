cmake_minimum_required(VERSION 3.7)
project(RLSnakeGame)

set(CMAKE_CXX_STANDARD 17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(Curses REQUIRED)

file(GLOB SOURCES "src/*.cpp")
include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CURSES_INCLUDE_DIRS})

add_subdirectory(src)
set(BUILD_TESTS NO CACHE BOOL "Build test target")
if (BUILD_TESTS)
    add_library(
            snake_game
            src/game.cpp
            src/snake.cpp
            src/food.cpp
            src/game_environment.cpp
            src/game_simulator.cpp
            src/io.cpp
    )
    add_library(
            gui
            src/menu.cpp
            src/gui.cpp
    )
    add_subdirectory(tests)
    add_subdirectory(tests/unit)
    add_subdirectory(tests/integration)
    target_link_libraries(gui ${CURSES_LIBRARIES})
endif ()

add_executable(RLSnakeGame ${SOURCES})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(RLSnakeGame ${SDL2_LIBRARIES} ${CURSES_LIBRARIES})
