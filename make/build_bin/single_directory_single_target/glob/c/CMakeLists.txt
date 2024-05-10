# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(manual_single_target LANGUAGES C)

# list sources
file(GLOB _sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_executable(swap ${_sources})
