# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(hello-world LANGUAGES C)

add_subdirectory(lib)

include_directories(lib)

add_executable(hello-world main.c)

target_link_libraries(hello-world hello)
