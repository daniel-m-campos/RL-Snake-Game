include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(googletest)

add_executable(TEST test.cpp snake_tests.cpp policy_tests.cpp)
target_link_libraries(TEST snake_game gtest gtest_main pthread)
