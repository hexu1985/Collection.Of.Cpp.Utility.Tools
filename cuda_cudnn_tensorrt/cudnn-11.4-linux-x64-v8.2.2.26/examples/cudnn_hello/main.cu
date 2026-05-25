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
