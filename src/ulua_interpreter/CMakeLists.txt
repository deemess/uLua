cmake_minimum_required(VERSION 3.6)   # CMake version check
project(uluai)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

set(SOURCES
        src/uluai.c
        )

add_executable(uluai ${SOURCES})

target_link_libraries(uluai ulua_core)