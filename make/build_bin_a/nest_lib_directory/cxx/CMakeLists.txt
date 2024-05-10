# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(hello-world LANGUAGES CXX)

add_subdirectory(lib)

include_directories(lib)

add_executable(hello-world hello-world.cpp)

target_link_libraries(hello-world message)
