#include <iostream>
#include <fstream>
#include <cuda_runtime.h>
#include <NvInfer.h>

class Logger : public nvinfer1::ILogger {
    void log(Severity severity, const char* msg) noexcept override {
        if (severity <= Severity::kWARNING)
            std::cout << "[TensorRT] " << msg << std::endl;
    }
};

// 辅助函数创建 Weights（注意保持数据生命周期）
nvinfer1::Weights makeWeights(const float* ptr, int64_t size) {
    nvinfer1::Weights w;
    w.count = size;
    w.type = nvinfer1::DataType::kFLOAT;
    w.values = ptr;
    return w;
}

int main() {
    Logger logger;
    auto builder = nvinfer1::createInferBuilder(logger);
    auto config = builder->createBuilderConfig();
    auto network = builder->createNetworkV2(1U); // 显式批处理

    const int inputSize = 3;
    const int outputSize = 2;

    // 权重和偏置
    float weightVals[] = {1.0f, 2.0f, 0.5f,  0.1f, 0.2f, 0.5f}; // [2, 3] row-major
    float biasVals[]   = {0.3f, 0.8f};
    auto weights = makeWeights(weightVals, 6);
    auto biases  = makeWeights(biasVals, 2);

    // 输入张量 [batch=1, C=3, H=1, W=1] 或者直接 [1,3] (使用 Dims2)
    // 这里为方便矩阵乘法，将输入reshape为 [1, 3]
    auto input = network->addInput("input", nvinfer1::DataType::kFLOAT,
                                    nvinfer1::Dims2{1, inputSize});  // [B, inputSize]
    if (!input) return -1;

    // ----- 1. 矩阵乘法：output = input * weight^T -----
    // 权重需作为常量层 (IConstantLayer) 参与计算
    auto weightConst = network->addConstant(nvinfer1::Dims2{outputSize, inputSize}, weights);
    auto matmul = network->addMatrixMultiply(*input, nvinfer1::MatrixOperation::kNONE,
                                             *weightConst->getOutput(0), nvinfer1::MatrixOperation::kTRANSPOSE);
    // matmul 输出形状 [B, outputSize]

    // ----- 2. 加上偏置 -----
    auto biasConst = network->addConstant(nvinfer1::Dims2{1, outputSize}, biases);
    // 使用 element-wise 加法 (广播)
    auto addBias = network->addElementWise(*matmul->getOutput(0), *biasConst->getOutput(0),
                                            nvinfer1::ElementWiseOperation::kSUM);

    // ----- 3. Sigmoid 激活 -----
    auto prob = network->addActivation(*addBias->getOutput(0), nvinfer1::ActivationType::kSIGMOID);
    prob->setName("sigmoid_output");

    // 标记输出
    network->markOutput(*prob->getOutput(0));

    // ----- 4. 构建引擎 -----
    config->setMemoryPoolLimit(nvinfer1::MemoryPoolType::kWORKSPACE, 1 << 28);
    auto engine = builder->buildSerializedNetwork(*network, *config);
    if (!engine) {
        std::cerr << "Engine 构建失败！" << std::endl;
        return -1;
    }

    // 保存引擎
    std::ofstream engineFile("simple_engine.engine", std::ios::binary);
    if (!engineFile) {
        std::cerr << "无法创建引擎文件！" << std::endl;
        return -1;
    }
    engineFile.write(reinterpret_cast<const char*>(engine->data()), engine->size());
    engineFile.close();

    std::cout << "引擎成功保存为 simple_engine.engine" << std::endl;

    delete engine;
    delete config;
    delete builder;
    return 0;
}
