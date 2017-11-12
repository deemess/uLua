cmake_minimum_required(VERSION 3.5)   # CMake version check
project(uluac)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

set(SOURCES
        src/uluac.c
        )

add_executable(uluac ${SOURCES})

target_link_libraries(uluac ulua_core)