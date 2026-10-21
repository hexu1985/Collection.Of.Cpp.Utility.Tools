# orin-toolchain.cmake
# 目标系统信息
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 工具链根目录：请修改为你的实际解压路径
set(TOOLCHAIN_ROOT "/opt/aarch64-buildroot-gcc-9.3.0")

# 编译器前缀（注意结尾的连字符）
set(TOOLCHAIN_PREFIX "aarch64-buildroot-linux-gnu-")

# 指定 C/C++ 编译器
set(CMAKE_C_COMPILER   "${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}g++")

# 指定 binutils
set(CMAKE_AR      "${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}ar"      CACHE FILEPATH "Archiver")
set(CMAKE_RANLIB  "${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}ranlib"  CACHE FILEPATH "Ranlib")
set(CMAKE_STRIP   "${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}strip"   CACHE FILEPATH "Strip")
set(CMAKE_LINKER  "${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}ld"      CACHE FILEPATH "Linker")

# 目标环境根目录（如果需要链接目标板上的库，可设置此路径）
# 通常需要从 Orin 设备上拷贝 /usr/lib 等目录到宿主机
# set(CMAKE_SYSROOT "${TOOLCHAIN_ROOT}/aarch64-buildroot-linux-gnu/sysroot")

# 搜索规则：只在工具链路径和目标根路径中查找库和头文件
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
