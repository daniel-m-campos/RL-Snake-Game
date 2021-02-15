cmake_minimum_required(VERSION 3.7)
project(RLSnakeGame)

set(CMAKE_CXX_STANDARD 17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)

file(GLOB SOURCES "src/*.cpp")
include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(src)
set(BUILD_TESTS NO CACHE BOOL "Build test target")
if (BUILD_TESTS)
    add_library(
            snake_game
            src/game.cpp
            src/snake.cpp
            src/policy.cpp
            src/action_valuer.cpp
            src/learner.cpp
            src/agent.cpp
            src/rl_factory.cpp
    )
    add_subdirectory(tests)
endif ()

add_executable(RLSnakeGame ${SOURCES})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(RLSnakeGame ${SDL2_LIBRARIES})
