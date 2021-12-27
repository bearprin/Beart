cmake_minimum_required(VERSION 3.21)
project(Beart)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TEST "BUILD_UNIT_TEST" OFF)

add_subdirectory(src/core)
if (BUILD_TEST)
    add_subdirectory(tests)
endif ()
add_subdirectory(third_party/Eigen)
add_subdirectory(third_party/Catch2)


