cmake_minimum_required(VERSION 3.12.4)
project(fastcdr_simple VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Fast CDR
find_package(fastcdr REQUIRED)

add_executable(demo demo.cpp)
target_link_libraries(demo PRIVATE fastcdr)

add_executable(string_demo string_demo.cpp)
target_link_libraries(string_demo PRIVATE fastcdr)

add_executable(serialize_demo serialize_demo.cpp main.cpp)
target_link_libraries(serialize_demo PRIVATE fastcdr)

