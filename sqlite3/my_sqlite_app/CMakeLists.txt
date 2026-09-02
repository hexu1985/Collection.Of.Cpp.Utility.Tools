cmake_minimum_required(VERSION 3.15)
project(my_sqlite_app VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 使用Conan生成的CMake配置
find_package(SQLite3 REQUIRED)

# 添加可执行文件
add_executable(my_sqlite_app src/main.cpp)

# 链接SQLite3库
target_link_libraries(my_sqlite_app 
    SQLite::sqlite3
)

# 如果有pthread依赖（Linux/Unix）
if(UNIX AND NOT APPLE)
    target_link_libraries(my_sqlite_app pthread)
endif()
