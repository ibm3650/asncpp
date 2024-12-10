cmake_minimum_required(VERSION 3.30)
if (MSVC)
    add_compile_options(/utf-8)
endif()
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

#add_executable(asncpp_main main.cpp)
#target_link_libraries(asncpp_main PRIVATE asncpp)


#add_executable(asncpp main.cpp
        #        include/asn_base.h
        #        types/integer.cpp
        #        types/integer_base.h
        #        types/boolean.h
        #        types/object_identifier.h
        #        types/enumerated.h
        #        types/relative_oid.h
        #        types/octet_string.h
        #        types/null.h
        #        types/bit_string.h
        #        types/object_descriptor.h
        #        types/utf_string.h
        #        types/numeric_string.h
        #        types/printable_string.h
        #        types/bmp_string.h
        #        types/visible_string.h
        #        types/ia5_string.h
        #        types/universal_string.h
        #        types/generalized_time.h
        #        types/utc_time.h
        #        types/date.h
        #        types/time_of_day.h
        #        types/date_time.h
        #        types/duration.h
#)
#if(BUILD_TESTS)
add_subdirectory(test)

#endif()

