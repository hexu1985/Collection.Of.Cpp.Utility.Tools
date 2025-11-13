cmake_minimum_required( VERSION 3.15 )

project ( span_demo LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB headers *.h)
add_executable(chapter06 main.cpp ${headers})
include_directories(${CMAKE_SOURCE_DIR}/include)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()
