cmake_minimum_required(VERSION 3.25)
project(Beart)

set(CMAKE_CXX_STANDARD 17)

# collect the third-party
set(BERT_PUBLIC_LIBS)
set(BERT_PRIVATE_LIBS)

option(BERT_ENABLE_OIDN "Enable OpenImageDenoise" ON)

find_package(TBB)
# if not found tbb, set BERT_ENABLE_OIDN OFF
if (NOT TBB_FOUND)
    message(WARNING "TBB not found, disable OpenImageDenoise")
    set(BERT_ENABLE_OIDN OFF CACHE FORCE)
else ()
    message(STATUS "TBB found, enable OpenImageDenoise")
endif ()

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

# tinyexr
add_subdirectory(third_party/tinyexr)
set(TINYEXR_INCLUDE_DIRS third_party/tinyexr)
include_directories(${TINYEXR_INCLUDE_DIRS})
list(APPEND BERT_PRIVATE_LIBS tinyexr)


# pcg
set(PCG_INCLUDE_DIRS third_party/pcg-cpp/include)
include_directories(${PCG_INCLUDE_DIRS})
add_library(pcg INTERFACE IMPORTED)
target_include_directories(pcg INTERFACE ${PCG_INCLUDE_DIRS})
list(APPEND BERT_PRIVATE_LIBS pcg)

# assimp
add_subdirectory(third_party/assimp)
set(ASSIMP_INCLUDE_DIRS third_party/assimp/include)
include_directories(${ASSIMP_INCLUDE_DIRS})
list(APPEND BERT_PRIVATE_LIBS assimp)

# nanothread
add_subdirectory(third_party/nanothread)
set(NANOTHREAD_INCLUDE_DIRS third_party/nanothread/include)
include_directories(${NANOTHREAD_INCLUDE_DIRS})
list(APPEND BERT_PRIVATE_LIBS nanothread)

# nlohmann json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(third_party/json)
list(APPEND BERT_PRIVATE_LIBS nlohmann_json::nlohmann_json)

# spdlog
add_subdirectory(third_party/spdlog)
list(APPEND BERT_PRIVATE_LIBS spdlog::spdlog)

# CLI11
add_subdirectory(third_party/CLI11)
list(APPEND BERT_PRIVATE_LIBS CLI11::CLI11)
add_subdirectory(src)


# add_subdirectory oidn
if (${BERT_ENABLE_OIDN})
    message(STATUS "OpenImageDenoise enabled")
    add_compile_definitions(BERT_ENABLE_OIDN)
    add_subdirectory(third_party/oidn)
endif ()



