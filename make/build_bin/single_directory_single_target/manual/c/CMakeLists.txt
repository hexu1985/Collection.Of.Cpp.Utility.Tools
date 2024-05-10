# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(manual_single_target LANGUAGES C)

# list sources
list(APPEND _sources main.c swapper.c)

add_executable(swap ${_sources})
