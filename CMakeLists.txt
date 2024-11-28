cmake_minimum_required(VERSION 3.30)
project(asncpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(asncpp main.cpp
        asn1/base.h
        types/integer.cpp
        types/integer.h
        types/boolean.h
        types/object_identifier.h
        types/enumerated.h
        types/relative_oid.h
        types/octet_string.h
        types/null.h
        types/bit_string.h
        types/object_descriptor.h)
