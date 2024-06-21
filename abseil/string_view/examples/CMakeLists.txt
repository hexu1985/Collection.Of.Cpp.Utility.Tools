cmake_minimum_required(VERSION 3.10)
project(string_view)

# Pick the C++ standard to compile with.
# Abseil currently supports C++14, C++17, and C++20.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(abseil-cpp)

add_executable(string_view1 string_view1.cpp)
target_link_libraries(string_view1 absl::strings)
