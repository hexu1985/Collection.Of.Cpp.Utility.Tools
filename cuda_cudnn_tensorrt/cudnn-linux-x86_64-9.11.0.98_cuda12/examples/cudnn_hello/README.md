这里是一个 cuDNN (CUDNN) "Hello World" 项目示例。它实现了一个简单的Sigmoid激活函数计算，可以让你直观地体验cuDNN库的基本工作流程。通常，这种级别的任务是验证 cuDNN 环境是否配置成功的有效手段。

### 项目结构
```
cudnn_hello/
├── CMakeLists.txt   # CMake 构建配置
└── main.cu          # 主程序源码
```

### 1. 源代码 (`main.cu`)

```cpp
#include <iostream>
#include <cuda_runtime.h>
#include <cudnn.h>

/**
 * cuDNN 最小示例：对张量应用 Sigmoid 激活函数。
 * 该示例将展示如何初始化 cuDNN、创建张量描述符、执行计算并清理资源。
 */
int main() {
    // 1. 获取 GPU 基本信息
    int numGPUs = 0;
    cudaGetDeviceCount(&numGPUs);
    std::cout << "找到 " << numGPUs << " 个 GPU。" << std::endl;
    if (numGPUs == 0) {
        std::cerr << "未找到支持 CUDA 的设备，程序退出。" << std::endl;
        return -1;
    }
    cudaSetDevice(0); // 使用 GPU 0
    
    int device = 0;
    struct cudaDeviceProp devProp;
    cudaGetDeviceProperties(&devProp, device);
    std::cout << "当前 GPU 计算能力: " << devProp.major << "." << devProp.minor << std::endl;
    
    // 2. 创建 cuDNN 句柄
    cudnnHandle_t cudnnHandle;
    cudnnCreate(&cudnnHandle);
    std::cout << "cuDNN 句柄创建成功。" << std::endl;
    
    // 3. 定义输入张量的维度 (NCHW 格式)
    cudnnDataType_t dataType = CUDNN_DATA_FLOAT;
    cudnnTensorFormat_t tensorFormat = CUDNN_TENSOR_NCHW;
    int n = 1, c = 1, h = 1, w = 10;  // 共 1*1*1*10 = 10 个元素
    int numElements = n * c * h * w;
    
    // 4. 创建输入张量描述符
    cudnnTensorDescriptor_t inputTensorDesc;
    cudnnCreateTensorDescriptor(&inputTensorDesc);
    cudnnSetTensor4dDescriptor(inputTensorDesc, tensorFormat, dataType, n, c, h, w);
    
    // 5. 分配设备内存并初始化数据
    float *inputData = nullptr;
    cudaMallocManaged(&inputData, numElements * sizeof(float)); // Unified Memory
    
    // 初始化输入张量为 0, 1, 2, ..., 9
    for (int i = 0; i < numElements; i++) {
        inputData[i] = static_cast<float>(i);
    }
    std::cout << "输入张量: ";
    for (int i = 0; i < numElements; i++) {
        std::cout << inputData[i] << " ";
    }
    std::cout << std::endl;
    
    // 6. 创建 Sigmoid 激活函数描述符
    cudnnActivationDescriptor_t activationDesc;
    cudnnCreateActivationDescriptor(&activationDesc);
    cudnnActivationMode_t mode = CUDNN_ACTIVATION_SIGMOID;
    cudnnNanPropagation_t nanProp = CUDNN_NOT_PROPAGATE_NAN;
    double coef = 0.0; // 对于 Sigmoid 无效，仅用于其他激活函数
    cudnnSetActivationDescriptor(activationDesc, mode, nanProp, coef);
    
    // 7. 执行 Sigmoid 前向计算 (原地计算)
    float alpha = 1.0f;
    float beta = 0.0f;
    cudnnActivationForward(cudnnHandle, 
                           activationDesc,
                           &alpha, 
                           inputTensorDesc, 
                           inputData,
                           &beta,  
                           inputTensorDesc, 
                           inputData);
    
    // 8. 打印计算结果
    std::cout << "Sigmoid 输出: ";
    for (int i = 0; i < numElements; i++) {
        std::cout << inputData[i] << " ";
    }
    std::cout << std::endl;
    
    // 9. 清理资源
    cudnnDestroyActivationDescriptor(activationDesc);
    cudnnDestroyTensorDescriptor(inputTensorDesc);
    cudnnDestroy(cudnnHandle);
    cudaFree(inputData);
    
    std::cout << "资源清理完成，程序结束。" << std::endl;
    return 0;
}
```

### 2. CMake 配置文件 (`CMakeLists.txt`)

对于依赖 CUDA 的项目，CMake 能提供跨平台的构建管理。

```cmake
cmake_minimum_required(VERSION 3.12)
project(cudnn_hello LANGUAGES CXX CUDA)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 寻找 CUDA 工具包，并开启 CUDA 支持
find_package(CUDA REQUIRED)

# 寻找 cuDNN 库，由 NVIDIA 官方提供的 FindCUDNN.cmake 脚本支持
find_package(CUDNN REQUIRED)

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
```

### 编译与运行

1.  进入项目目录 `cudnn_hello`
2.  在终端执行以下命令：
    ```bash
    mkdir build && cd build
    cmake -DCUDNN_ROOT=/path/to/your/cudnn ..  # 若 cuDNN 未安装在标准路径，需指定根目录
    make
    ./cudnn_hello
    ```

**环境说明**：`find_package(CUDA)` 是 CMake 内置的模块，查找 CUDA 时依赖环境变量中的 CUDA 路径。而 `find_package(CUDNN)` 需要 NVIDIA 提供的 `FindCUDNN.cmake` 脚本，它会默认在 `CUDNN_ROOT` 等路径下搜索头文件和库文件。

程序成功运行后，会先输出 GPU 信息，然后对 0~9 这组数字依次计算 Sigmoid 函数，最终输出计算结果。这个简单的项目验证了 cuDNN 库的初始化、张量描述符的创建、激活函数算子的构造与执行，以及资源的正确释放，为后续开发更复杂的深度学习应用奠定了基础。

---

如果我的cudnn是通过tar包安装到cuda目录下的，没有FindCUDNN.cmake脚本，怎么修改CMakeLists.txt，不通过find_package(CUDNN)方式，可以直接找的CUDNN的头文件和库吗？

如果你的 cuDNN 是通过 tar 包直接解压并复制到 CUDA 目录下的（例如头文件在 `/path/to/cuda/include`，库文件在 `/path/to/cuda/lib64`），那么你不需要 `FindCUDNN.cmake` 脚本。你可以直接利用 CMake 已经找到的 CUDA 路径，手动指定 cuDNN 的头文件和库文件。

下面是一个修改后的 `CMakeLists.txt` 示例，它**不依赖** `find_package(CUDNN)`，而是直接基于 `CUDA_TOOLKIT_ROOT_DIR`（由 `find_package(CUDA)` 设置）来定位 cuDNN。

### 修改后的 `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.12)
project(cudnn_hello LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 寻找 CUDA（这会设置 CUDA_TOOLKIT_ROOT_DIR 等变量）
find_package(CUDA REQUIRED)

# 2. 手动设置 cuDNN 头文件路径和库路径
# 假设 cuDNN 的头文件在 ${CUDA_TOOLKIT_ROOT_DIR}/include
# 库文件在 ${CUDA_TOOLKIT_ROOT_DIR}/lib64 （或 lib）
set(CUDNN_INCLUDE_DIR ${CUDA_TOOLKIT_ROOT_DIR}/include)
set(CUDNN_LIBRARY ${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudnn.so)

# 如果 cuDNN 安装在其他自定义位置，可以直接写绝对路径，例如：
# set(CUDNN_INCLUDE_DIR /home/user/my_cuda/include)
# set(CUDNN_LIBRARY /home/user/my_cuda/lib64/libcudnn.so)

# 检查头文件和库是否存在（可选，用于调试）
if(NOT EXISTS ${CUDNN_INCLUDE_DIR}/cudnn.h)
    message(FATAL_ERROR "cudnn.h not found in ${CUDNN_INCLUDE_DIR}")
endif()
if(NOT EXISTS ${CUDNN_LIBRARY})
    message(FATAL_ERROR "libcudnn.so not found at ${CUDNN_LIBRARY}")
endif()

# 3. 添加可执行文件
cuda_add_executable(cudnn_hello main.cu)

# 4. 包含头文件目录并链接库
target_include_directories(cudnn_hello PRIVATE ${CUDNN_INCLUDE_DIR})
target_link_libraries(cudnn_hello ${CUDA_LIBRARIES} ${CUDNN_LIBRARY})
```

### 几点说明

1. **路径假设**：上述代码假设你的 CUDA 安装在标准的 `CUDA_TOOLKIT_ROOT_DIR`（例如 `/usr/local/cuda` 或你自定义的路径），且 cuDNN 的头文件和库已经分别放在 `include/` 和 `lib64/` 下。如果你的 `lib64/` 下还有别的子目录，请相应调整。

2. **库文件名**：Linux 下通常为 `libcudnn.so`，如果版本带有后缀（如 `libcudnn.so.8`），也可以直接写 `libcudnn.so`（符号链接）。你也可以使用 `find_library` 更灵活地搜索：
   ```cmake
   find_library(CUDNN_LIBRARY NAMES cudnn libcudnn PATHS ${CUDA_TOOLKIT_ROOT_DIR}/lib64)
   ```

3. **多版本管理**：如果你有多个 CUDA 版本，可以在运行 CMake 时通过 `-DCUDA_TOOLKIT_ROOT_DIR=/path/to/your/cuda` 指定，cuDNN 路径会自动跟随。

4. **Windows 注意事项**：若在 Windows 下，库名可能是 `cudnn.lib`，路径可能为 `lib/x64`，请相应修改。

这样修改后，你的项目就能在不依赖 `FindCUDNN.cmake` 的情况下正常编译链接 cuDNN 程序了。
