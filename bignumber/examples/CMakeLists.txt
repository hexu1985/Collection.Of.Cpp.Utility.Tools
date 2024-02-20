# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(bignumber_examples LANGUAGES CXX)

# require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/BigNumber)

add_executable(demo demo.cpp)
target_include_directories(demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/BigNumber/src)
target_link_libraries(demo PRIVATE BigNumber)

add_executable(fig03_05 fig03_05.cpp)
target_include_directories(fig03_05 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/BigNumber/src)
target_link_libraries(fig03_05 PRIVATE BigNumber)
