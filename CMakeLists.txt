
#Что происходит?
#target_include_directories() добавляет пути к заголовочным файлам (.h) для цели asncpp.
#
#Параметры
#PUBLIC — Указывает, что эти include-директории будут видны:
#При сборке текущей библиотеки.
#При сборке проектов, которые будут использовать эту библиотеку.
#Разбор путей
#$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#➔ Этот путь используется только во время сборки проекта.
#🔹 Это полезно, чтобы проекты, включающие вашу библиотеку в виде исходников (например, через add_subdirectory()), видели правильный путь к заголовочным файлам.
#
#$<INSTALL_INTERFACE:include>
#➔ Этот путь используется только после установки библиотеки.
#🔹 Он указывает на путь, где заголовочные файлы окажутся после команды make install.
#
#${CMAKE_CURRENT_SOURCE_DIR}/types
#➔ Этот путь добавляется всегда (и при сборке, и после установки).
#🔹 Это уместно, если каталог types содержит внутренние заголовочные файлы, которые не переносятся при установке.
#
#Почему нужно использовать BUILD_INTERFACE и INSTALL_INTERFACE?
#Без них можно столкнуться с проблемой, когда при установке библиотеки пути остаются жёстко привязанными к исходному каталогу.
#Эта конструкция обеспечивает корректную установку и корректную работу как при локальной разработке, так и при установке библиотеки.

cmake_minimum_required(VERSION 3.29)
project(asncpp VERSION 1.0.0)

# Стандарты компиляции
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Предупреждения
if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

# Добавляем библиотеку
add_library(
        asncpp
        src/common.cpp
        src/asn1_basic.cpp
        src/integer_basic.cpp
        src/boolean.cpp
        src/bit_string.cpp
)

# Пути к заголовочным файлам
target_include_directories(asncpp
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Поддержка установки и экспорта
include(GNUInstallDirs)

install(TARGETS asncpp
        EXPORT asncppTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Установка заголовочных файлов
install(
        DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Экспорт целей
install(
        EXPORT asncppTargets
        NAMESPACE asncpp::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/asncpp
)

# Конфигурация пакета
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/asncppConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
)

configure_file(
        cmake/asncppConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/asncppConfig.cmake"
        @ONLY
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/asncppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/asncppConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/asncpp
)

# Подключение тестов
add_subdirectory(test)


