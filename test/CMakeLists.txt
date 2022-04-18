project(unittest)

add_executable(unittest camera_test.cpp)

target_link_libraries(unittest PRIVATE Catch2::Catch2WithMain)
target_link_libraries(unittest PRIVATE Core_lib)
