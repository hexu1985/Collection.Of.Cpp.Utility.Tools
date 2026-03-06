cmake_minimum_required(VERSION 3.14)
project(Catch2ExampleV2 LANGUAGES CXX)

# 使用 C++14 标准（v2 通常兼容 C++14/11）
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加测试可执行文件
add_executable(tests test.cpp)

# 链接 Catch2 库（不含 main，main 由用户提供）
#target_link_libraries(tests PRIVATE Catch2::Catch2)

