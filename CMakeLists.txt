cmake_minimum_required(VERSION 3.10.0)
project(ecs VERSION 0.1.0 LANGUAGES C CXX)


add_subdirectory(third_party)
add_subdirectory(src)

target_include_directories(ecs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(ecs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party)



