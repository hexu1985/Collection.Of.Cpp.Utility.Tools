# 输出CMake版本提示
message(STATUS "The CMAKE_VERSION is ${CMAKE_VERSION}.")

# 指定CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义工程名称、版本和语言
project(sqlite3 VERSION 3.46.0 LANGUAGES C) # 请将版本号替换为实际下载的版本

# 设置构建选项
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_SQLITE3_SHELL "Build the sqlite3 shell executable" ON)

# 指定头文件目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 将源代码文件添加到库中
set(SOURCES
    src/sqlite3.c
)

# 创建库 (动态库或静态库，由 BUILD_SHARED_LIBS 控制)
add_library(sqlite3 ${SOURCES})

# 为库设置公开的头文件目录
target_include_directories(sqlite3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 如果是在Windows下构建动态库，需要链接模块定义文件
if(WIN32 AND BUILD_SHARED_LIBS)
    target_sources(sqlite3 PRIVATE sqlite3.def)
endif()

# 设置一些常用的编译选项 (可根据需要开启或关闭)
# 更多选项参考: https://www.sqlite.org/compile.html
target_compile_definitions(sqlite3 PRIVATE
    # 启用FTS5全文搜索扩展
    # SQLITE_ENABLE_FTS5
    # 启用RTREE空间索引扩展
    # SQLITE_ENABLE_RTREE
    # 启用JSON支持
    # SQLITE_ENABLE_JSON
)

# 如果需要构建命令行工具 sqlite3
if(BUILD_SQLITE3_SHELL)
    # 注意: shell.c 不在融合版中，需要从完整源码包或单独下载
    # 这里只是示例，实际使用时需要将 shell.c 放在正确的位置
    # add_executable(sqlite3-shell src/shell.c)
    # target_link_libraries(sqlite3-shell sqlite3)
    # 如果使用系统的 readline 库，还需要链接
    # target_link_libraries(sqlite3-shell sqlite3 readline)
    message(STATUS "SQLite3 shell build is not included in the amalgamation build. Please add shell.c manually.")
endif()

# 安装规则
install(TARGETS sqlite3
    EXPORT SQLite3Targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

install(FILES include/sqlite3.h include/sqlite3ext.h
    DESTINATION include
)

# 导出CMake包配置，方便其他项目使用 find_package 查找
install(EXPORT SQLite3Targets
    FILE SQLite3Config.cmake
    NAMESPACE SQLite::
    DESTINATION lib/cmake/SQLite3
)
