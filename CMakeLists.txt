cmake_minimum_required(VERSION 3.30)
if (MSVC)
    add_compile_options(/utf-8)
endif ()
project(asncpp)

set(CMAKE_CXX_STANDARD 23)
#add_library(asncpp INTERFACE)
add_library(
        asncpp
        main.cpp
)


target_include_directories(asncpp
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/types
)

add_subdirectory(test)


