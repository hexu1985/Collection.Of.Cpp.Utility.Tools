# 指定目标系统
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 指定交叉编译器
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# 指定编译器和链接器选项（可选）
#set(CMAKE_C_FLAGS "-march=armv7-a" CACHE STRING "" FORCE)
#set(CMAKE_CXX_FLAGS "-march=armv7-a" CACHE STRING "" FORCE)

# 指定sysroot（如果有）
# set(CMAKE_SYSROOT /path/to/sysroot)

# 指定查找程序时，不查找宿主系统
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
