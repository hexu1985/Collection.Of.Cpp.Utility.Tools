# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(Visitor LANGUAGES CXX)

# list sources
list(APPEND _sources 
    Directory.cpp
    Directory.hpp
    Element.cpp
    Element.hpp
    Entry.cpp
    Entry.hpp
    File.cpp
    File.hpp
    ListVisitor.cpp
    ListVisitor.hpp
    Main.cpp
    Visitor.cpp
    Visitor.hpp
    )

add_executable(Visitor ${_sources})
