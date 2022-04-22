cmake_minimum_required(VERSION 3.21)
project(Beart)

set(CMAKE_CXX_STANDARD 17)
option(BUILD_TESTS "Build tests" ON)
option(FAST_MATH "Use fast math" ON)
add_subdirectory(third_party/Eigen)
add_definitions(-DEIGEN)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(third_party/Catch2)
    add_subdirectory(test)
endif ()

if (FAST_MATH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
endif ()

# backward
if (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo" OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Debug build, Building with backward-cpp")
    add_subdirectory(third_party/backward-cpp)
endif ()

add_subdirectory(third_party)
add_subdirectory(src/core)

