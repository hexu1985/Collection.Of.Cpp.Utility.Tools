cmake_minimum_required(VERSION 3.12.4)
project(fastcdr_simple VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Werror=return-type)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

if(NOT tinyxml2_FOUND)
    find_package(tinyxml2 REQUIRED)
endif()

add_executable(test_ClientServerTypes test_ClientServerTypes.cpp
    gen/ClientServerTypes.cxx
    gen/ClientServerTypes.h
    test_ClientServerTypes.hpp
    )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gen)

target_link_libraries(test_ClientServerTypes fastrtps fastcdr tinyxml2::tinyxml2)

