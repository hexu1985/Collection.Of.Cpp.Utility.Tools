# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(hello-world LANGUAGES CXX)

# generate a library from sources
add_library(message 
    SHARED 
    Message.hpp 
    Message.cpp
    )

add_executable(hello-world hello-world.cpp)

target_link_libraries(hello-world message)
