cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_CXX_STANDARD 17)

project(utf8_console)

if (WIN32)
    add_compile_options("/utf-8")
endif ()

add_library(utf8_console
    src/utf8_console.cpp

    src/wcwidth.c
    src/utfwidth.cpp
    src/utf8_cin_windows.cpp
)

target_include_directories(utf8_console PUBLIC "include")

add_executable(utf8_console_example example/example.cpp)
target_link_libraries(utf8_console_example utf8_console)