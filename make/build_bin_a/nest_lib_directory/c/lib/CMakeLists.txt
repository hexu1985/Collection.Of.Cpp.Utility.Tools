# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(hello-lib LANGUAGES C)

# generate a library from sources
add_library(hello 
    STATIC 
    hello.h
    hello.c
    )
