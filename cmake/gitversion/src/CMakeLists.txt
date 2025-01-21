cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(gitversion C)

# 设置自定义模块文件的所在路径
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

# 导入自定义模型文件
include(GitVersion)
include(BuildNumber)

# 获取程序的版本信息
fetch_version_from_git("GITVERSION")

# 获取程序的构建号
new_build_number("GITVERSION")

set(GITVERSION_VERSION "${GITVERSION_VERSION_MAJOR}-${GITVERSION_VERSION_MINOR}-${GITVERSION_VERSION_PATCH}-${GITVERSION_VERSION_STAGE}")

# 生成程序的构建时间
string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")

# 利用config.h.in模板文件自动生成config.h文件
configure_file("${PROJECT_SOURCE_DIR}/config.h.in"
            "${PROJECT_SOURCE_DIR}/config.h" @ONLY)

# 创建构建目标为可执行程序
add_executable(${PROJECT_NAME} test.c)
