cmake_minimum_required(VERSION 3.6)   # CMake version check
project(ulua)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

set(SOURCES
        src/ulua.c
        )

add_executable(ulua ${SOURCES})

target_link_libraries(ulua ulua_core)