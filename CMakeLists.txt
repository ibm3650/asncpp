cmake_minimum_required(VERSION 3.30)
project(asncpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
        asncpp
        main.cpp
        src/common.cpp
        src/asn1_basic.cpp
)

target_include_directories(asncpp
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/types
)

add_subdirectory(test)


