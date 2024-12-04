cmake_minimum_required(VERSION 3.29)
project(asncpp_test)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(asncpp_test ${TEST_SOURCES})
target_link_libraries(
        asncpp_test PRIVATE
        asncpp
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(asncpp_test)