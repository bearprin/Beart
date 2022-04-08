cmake_minimum_required(VERSION 3.21)
project(Beart)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Debug build, Building with backward-cpp")
    add_subdirectory(third_party/backward-cpp)
endif ()

add_subdirectory(src/core)
add_subdirectory(third_party/Eigen)


