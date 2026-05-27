cmake_minimum_required(VERSION 3.12)
project(tensorrt_hello)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 抑制废弃API警告
add_compile_options(-Wno-deprecated-declarations)

# 请修改为你的TensorRT 10.16.1.11实际安装路径
set(TensorRT_ROOT "/home/hexu/local/TensorRT-10.16.1.11")

find_package(CUDA REQUIRED)

set(TensorRT_INCLUDE_DIR ${TensorRT_ROOT}/include)
set(TensorRT_LIB_DIR ${TensorRT_ROOT}/lib)

include_directories(${TensorRT_INCLUDE_DIR})
include_directories(${CUDA_INCLUDE_DIRS})
link_directories(${TensorRT_LIB_DIR})

# 将 main_v1.cpp 或 main_v2.cpp 重命名为 main.cpp 进行编译
add_executable(tensorrt_hello main.cpp)

target_link_libraries(tensorrt_hello nvinfer)
