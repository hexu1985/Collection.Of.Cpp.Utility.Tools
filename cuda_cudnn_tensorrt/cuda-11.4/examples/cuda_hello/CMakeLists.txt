cmake_minimum_required(VERSION 3.12)
project(CUDA_Hello LANGUAGES CXX CUDA)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(cuda_hello main.cu)

# 如果 CUDA 需要显式链接运行时库（通常自动处理，以下行可选）
# target_link_libraries(cuda_hello CUDA::cudart)
