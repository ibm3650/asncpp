cmake_minimum_required(VERSION 3.29)
project(asncpp_tests)

include("../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
endif()
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)





set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # do not override parent project's runtime settings on Windows
set(INSTALL_GTEST OFF)
set(BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(googletest)


if(is_top_level)
    find_package(asncpp REQUIRED)
endif()

set(TEST_SOURCES
        asn_basic_test.cpp
        integer_enumerated_test.cpp
        common_test.cpp

)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_SOURCES})
add_executable(asncpp-tests)
target_sources(asncpp-tests PRIVATE ${TEST_SOURCES})

target_link_libraries(asncpp-tests
        PRIVATE
        asncpp::asncpp
        gtest_main)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(asncpp-tests asncpp::asncpp)
endif()

include(GoogleTest)
gtest_discover_tests(asncpp-tests)

