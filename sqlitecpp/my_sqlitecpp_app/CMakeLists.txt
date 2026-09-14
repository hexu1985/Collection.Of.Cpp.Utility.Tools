cmake_minimum_required(VERSION 3.15)
project(my_sqlitecpp_app VERSION 1.0.0 LANGUAGES CXX)

# C++11 或更高（SQLiteCpp 要求 C++11）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 SQLiteCpp —— 由 CMakeDeps 生成的 SQLiteCppConfig.cmake 提供
find_package(SQLiteCpp REQUIRED)

# 添加可执行文件
add_executable(my_sqlitecpp_app src/main.cpp)

# 链接 SQLiteCpp
# 注意：SQLiteCpp 的 CMake 导出目标名就是 SQLiteCpp（没有命名空间）
target_link_libraries(my_sqlitecpp_app PRIVATE SQLiteCpp)

# Unix 下需要 pthread 和 dl（SQLiteCpp 的 package_info 已声明，但显式加上更保险）
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(my_sqlitecpp_app PRIVATE Threads::Threads ${CMAKE_DL_LIBS})
endif()
