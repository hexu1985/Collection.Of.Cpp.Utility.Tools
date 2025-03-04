cmake_minimum_required(VERSION 3.10)
project(string_view)

# Pick the C++ standard to compile with.
# Abseil currently supports C++14, C++17, and C++20.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(absl REQUIRED)

add_executable(crc_demo crc_demo.cpp)
target_link_libraries(crc_demo absl::crc32c)
