include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(googletest)

add_executable(
        UNITTEST
        test.cpp
        snake_tests.cpp
        policy_tests.cpp
        action_valuer_tests.cpp
        learner_tests.cpp
        agent_tests.cpp
        rl_factory_tests.cpp
)

target_link_libraries(UNITTEST snake_game gtest gmock gtest_main pthread)
