cmake_minimum_required(VERSION 3.12)
project(cudnn_hello LANGUAGES CXX CUDA)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 寻找 CUDA 工具包，并开启 CUDA 支持
find_package(CUDA REQUIRED)

# 寻找 cuDNN 库，由 NVIDIA 官方提供的 FindCUDNN.cmake 脚本支持
#find_package(CUDNN REQUIRED)
set(CUDNN_INCLUDE_DIR ${CUDA_TOOLKIT_ROOT_DIR}/include)
set(CUDNN_LIBRARIES ${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudnn.so)

# 检查头文件和库是否存在（可选，用于调试）
if(NOT EXISTS ${CUDNN_INCLUDE_DIR}/cudnn.h)
    message(FATAL_ERROR "cudnn.h not found in ${CUDNN_INCLUDE_DIR}")
endif()
if(NOT EXISTS ${CUDNN_LIBRARIES})
    message(FATAL_ERROR "libcudnn.so not found at ${CUDNN_LIBRARY}")
endif()

# 添加可执行文件
cuda_add_executable(cudnn_hello main.cu)

# 链接必要的库
target_link_libraries(cudnn_hello
    ${CUDA_LIBRARIES}
    ${CUDNN_LIBRARIES}
)

# 包含必要的头文件目录
target_include_directories(cudnn_hello PRIVATE
    ${CUDA_INCLUDE_DIRS}
    ${CUDNN_INCLUDE_DIR}
)
