cmake_minimum_required(VERSION 3.6)   # CMake version check
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

include_directories(src/ulua_core/include)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

add_subdirectory(src/ulua_core)
add_subdirectory(src/ulua_repl)
add_subdirectory(src/ulua_compiler)
add_subdirectory(src/ulua_interpreter)

