cmake_minimum_required(VERSION 3.25)
project(Beart)

set(CMAKE_CXX_STANDARD 17)

# Enoki
add_subdirectory(third_party/enoki)
set(ENOKI_INCLUDE_DIRS third_party/enoki/include)
include_directories(${ENOKI_INCLUDE_DIRS})

add_subdirectory(src)


