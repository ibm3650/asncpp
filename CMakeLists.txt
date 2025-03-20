# Минимальная версия CMake
cmake_minimum_required(VERSION 3.29)

# Описание проекта
project(asncpp
        VERSION 0.1.0
        DESCRIPTION "ASN.1 library for C++"
        HOMEPAGE_URL "https://github.com/ibm3650/asncpp"
        LANGUAGES CXX
)

# Подключение вспомогательных функций
include(cmake/utils.cmake)
include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Определение переменной is_top_level. Она равна true, если проект собирается в корневой директории
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)


# Опции сборки. По умолчанию тесты, документация, цели для установки в систему и примеры не собираются.
option(ASNCPP_BUILD_TESTS "Build ASNCPP tests" OFF)
option(ASNCPP_BUILD_EXAMPLES "Build ASNCPP examples" OFF)
option(ASNCPP_BUILD_DOCS "Build ASNCPP documentation" OFF)
option(ASNCPP_INSTALL "Generate target for installing ASNCPP" ${is_top_level})

# Переменные для установки. По умолчанию устанавливаются в директорию lib/cmake/ASNCPP
set_if_undefined(ASNCPP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/ASNCPP" CACHE STRING
        "Install path for ASNCPP package-related CMake files")


# Переменные для установки. Собирать ли динамические библиотеки
if (DEFINED ASNCPP_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${ASNCPP_SHARED_LIBS})
endif ()

# Проверяем, не задан ли CMAKE_BUILD_TYPE (для Unix)
# и CMAKE_CONFIGURATION_TYPES (для Visual Studio, Xcode)
if (NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    # Если CMAKE_BUILD_TYPE не задан, устанавливаем его в "Release" по умолчанию
    # CACHE STRING — сохраняет значение в кэше CMake
    # FORCE — перезаписывает значение в кэше, если оно уже было установлено
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)

    # Указываем допустимые значения для CMAKE_BUILD_TYPE
    # Это полезно при использовании CMake GUI (чтобы можно было выбрать из списка)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()


set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)#?
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)#?

# Подключение библиотеки
add_library(asncpp) # Определена ниже
add_library(asncpp::asncpp ALIAS asncpp)#?

include(GenerateExportHeader)#?
set(export_file_name "export_shared.h")

if (NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif ()

generate_export_header(asncpp EXPORT_FILE_NAME include/asncpp/${export_file_name})

set(sources
        include/asncpp/export.h
        include/asncpp/base/common.h
        include/asncpp/base/asn1_basic.h
        include/asncpp/base/integer_basic.h
        include/asncpp/boolean.h
        include/asncpp/bit_string.h

        src/common.cpp
        src/asn1_basic.cpp
        src/integer_basic.cpp
        src/boolean.cpp
        src/bit_string.cpp

)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})


include(CMakePackageConfigHelpers)

target_sources(asncpp PRIVATE ${sources})
target_compile_definitions(asncpp PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:ASNCPP_STATIC_DEFINE>")

target_include_directories(asncpp
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

set_target_properties(asncpp PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

if (ASNCPP_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/asncpp_config.cmake.in asncpp_config.cmake
            INSTALL_DESTINATION "${ASNCPP_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(asncpp_config_version.cmake
            COMPATIBILITY SameMajorVersion)

    install(TARGETS asncpp EXPORT asncpp_export
            RUNTIME COMPONENT asncpp
            LIBRARY COMPONENT asncpp NAMELINK_COMPONENT asncpp-dev
            ARCHIVE COMPONENT asncpp-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
            TYPE INCLUDE
            COMPONENT asncpp-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/asncpp/${export_file_name}"
            COMPONENT asncpp-dev
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/asncpp")

    set(targets_file "asncpp-shared-targets.cmake")

    if (NOT BUILD_SHARED_LIBS)
        set(targets_file "asncpp-static-targets.cmake")
    endif ()

    install(EXPORT asncpp_export
            COMPONENT asncpp-dev
            FILE "${targets_file}"
            DESTINATION "${ASNCPP_INSTALL_CMAKEDIR}"
            NAMESPACE asncpp::)

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/asncpp_config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/asncpp_config_version.cmake"
            COMPONENT asncpp-dev
            DESTINATION "${ASNCPP_INSTALL_CMAKEDIR}")

    if (MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if (BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:asncpp>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else ()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:asncpp>/$<TARGET_FILE_PREFIX:asncpp>$<TARGET_FILE_BASE_NAME:asncpp>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif ()

        install(FILES "${pdb_file}"
                COMPONENT asncpp-dev
                CONFIGURATIONS Debug RelWithDebInfo
                DESTINATION "${pdb_file_destination}"
                OPTIONAL)
    endif ()
endif ()

#----------------------------------------------------------------------------------------------------------------------
# other targets
#----------------------------------------------------------------------------------------------------------------------

#if (ASNCPP_BUILD_TESTS)
enable_testing()
add_subdirectory(tests)
#endif ()

#if (ASNCPP_BUILD_EXAMPLES)
add_subdirectory(examples)
#endif ()

#if (ASNCPP_BUILD_DOCS)
find_package(Doxygen REQUIRED)
#set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")
doxygen_add_docs(docs include)

#endif ()


