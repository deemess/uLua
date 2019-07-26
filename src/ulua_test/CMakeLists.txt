cmake_minimum_required(VERSION 3.5)   # CMake version check
project(uluatest)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

set(SOURCES
        src/uluatest.c
        )

add_executable(uluatest ${SOURCES})

target_link_libraries(uluatest ulua_core)