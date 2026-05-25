以下是一个简单的 CUDA Hello World 项目，包含源码和 CMake 配置文件。该程序会调用一个 GPU 核函数输出信息，并演示 CUDA 设备属性查询。

### 项目结构

```
cuda_hello/
├── CMakeLists.txt
└── main.cu
```

### 文件内容

#### `main.cu`

```cpp
#include <iostream>
#include <cuda_runtime.h>

// GPU 核函数：打印来自 GPU 的消息
__global__ void helloFromGPU() {
    printf("Hello World from GPU thread %d in block %d\n", threadIdx.x, blockIdx.x);
}

int main() {
    // 打印 CUDA 设备信息
    int deviceCount = 0;
    cudaGetDeviceCount(&deviceCount);
    if (deviceCount == 0) {
        std::cerr << "No CUDA-capable device detected!" << std::endl;
        return 1;
    }

    int deviceId = 0;
    cudaSetDevice(deviceId);

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, deviceId);
    std::cout << "Running on device: " << prop.name << std::endl;
    std::cout << "  Compute capability: " << prop.major << "." << prop.minor << std::endl;
    std::cout << "  Total global memory: " << prop.totalGlobalMem / (1024 * 1024) << " MB" << std::endl;

    // 启动核函数：1 个 block，每个 block 有 4 个线程
    helloFromGPU<<<1, 4>>>();
    cudaDeviceSynchronize();  // 等待 GPU 完成，确保 printf 输出

    std::cout << "CUDA kernel launched successfully!" << std::endl;
    return 0;
}
```

#### `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.12)
project(CUDA_Hello LANGUAGES CXX CUDA)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(cuda_hello main.cu)

# 如果 CUDA 需要显式链接运行时库（通常自动处理，以下行可选）
# target_link_libraries(cuda_hello CUDA::cudart)
```

### 编译与运行说明

1. **进入项目目录**
   ```bash
   cd cuda_hello
   ```

2. **配置 & 编译**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **运行程序** （需要有实际的 NVIDIA 显卡和驱动，否则会报错）
   ```bash
   ./cuda_hello
   ```

### 验证编译环境（无需 GPU）

如果你的目标仅仅是**编译**（不运行），那么上述配置会在有 CUDA Toolkit 的机器上成功生成 `cuda_hello` 可执行文件，即使没有物理显卡也可以完成编译。运行时会因为无法找到 GPU 而报错，但这不影响对编译环境的测试。

你可以在配置 CMake 后执行 `make` 来确认编译是否通过。