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
