# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(manual_multi_target LANGUAGES CXX)

add_executable(queue_back_test queue_back_test.cpp)
add_executable(queue_empty_test queue_empty_test.cpp)
add_executable(queue_front_test queue_front_test.cpp)
add_executable(queue_pop_test queue_pop_test.cpp)
