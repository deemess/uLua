cmake_minimum_required(VERSION 3.6)   # CMake version check
project(ulua_core)
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

set(ULUA_CORE_INCLUDE_DIR include)

set(ULUA_CORE_SOURCES
        src/gc.c
        src/native.c
        src/table.c
        src/ucodegen.c
        src/udebug.c
        src/ulexer.c
        src/umemory.c
        src/uparser.c
        src/vm.c
        )

include_directories(${ULUA_CORE_INCLUDE_DIR})

add_executable(lemon src/lemon.c)

configure_file(src/lempar.c lempar.c COPYONLY)

add_custom_target(lemon_run
        COMMAND lemon src/uparser.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Run lemon in ${CMAKE_CURRENT_SOURCE_DIR} to generate source"
        )

add_library(ulua_core ${ULUA_CORE_SOURCES} )

add_dependencies(ulua_core lemon_run)