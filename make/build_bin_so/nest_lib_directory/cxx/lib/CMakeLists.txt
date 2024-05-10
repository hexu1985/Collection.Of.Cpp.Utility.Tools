# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(message-lib LANGUAGES CXX)

# generate a library from sources
add_library(message 
    SHARED 
    Message.hpp 
    Message.cpp
    )
