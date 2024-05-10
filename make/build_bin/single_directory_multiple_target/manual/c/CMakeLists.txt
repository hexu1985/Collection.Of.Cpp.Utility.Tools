# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(manual_multi_target LANGUAGES C)

add_executable(fibonacci fibonacci.c)
add_executable(test1 test1.c)
add_executable(watch watch.c)
