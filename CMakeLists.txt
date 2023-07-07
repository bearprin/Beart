cmake_minimum_required(VERSION 3.25)
project(Beart)

set(CMAKE_CXX_STANDARD 17)

# collect the third-party
set(BERT_PUBLIC_LIBS)
set(BERT_PRIVATE_LIBS)
# Enoki
add_subdirectory(third_party/enoki)
set(ENOKI_INCLUDE_DIRS third_party/enoki/include)
include_directories(${ENOKI_INCLUDE_DIRS})

# stb
set(STB_INCLUDE_DIRS third_party/stb)
add_library(stb INTERFACE IMPORTED)
target_include_directories(stb INTERFACE ${STB_INCLUDE_DIRS})
list(APPEND BERT_PRIVATE_LIBS stb)
add_definitions(-DSTB_IMAGE_WRITE_IMPLEMENTATION -DSTB_IMAGE_WRITE_STATIC)

# pcg
set(PCG_INCLUDE_DIRS third_party/pcg-cpp/include)
include_directories(${PCG_INCLUDE_DIRS})
add_library(pcg INTERFACE IMPORTED)
target_include_directories(pcg INTERFACE ${PCG_INCLUDE_DIRS})
list(APPEND BERT_PRIVATE_LIBS pcg)

add_subdirectory(src)


