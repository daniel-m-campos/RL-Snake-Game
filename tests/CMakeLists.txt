include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
    SYSTEM
)
FetchContent_MakeAvailable(googletest)

# Suppress warnings in GoogleTest's own compilation.
# The SYSTEM keyword above suppresses warnings when project code includes gtest headers;
# this loop suppresses warnings emitted while building gtest/gmock themselves.
foreach(_gtest_target IN ITEMS gtest gtest_main gmock gmock_main)
    if(TARGET ${_gtest_target})
        target_compile_options(${_gtest_target} PRIVATE -w)
    endif()
endforeach()

include(GoogleTest)

add_subdirectory(unit)
add_subdirectory(integration)
