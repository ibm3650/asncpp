cmake_minimum_required(VERSION 3.22)

project(asncpp-integer
        VERSION 0.1.0
        DESCRIPTION "ASN.1 integer type support example"
        HOMEPAGE_URL "https://github.com/ibm3650/asncpp"
        LANGUAGES CXX
)

include("../../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if (is_top_level)
    find_package(asncpp REQUIRED)
endif ()

set(sources main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})
add_executable(asncpp-integer)
target_sources(asncpp-integer PRIVATE ${sources})
target_link_libraries(asncpp-integer PRIVATE asncpp::asncpp)

if (NOT is_top_level)
    win_copy_deps_to_target_dir(asncpp-integer asncpp::asncpp)
endif ()