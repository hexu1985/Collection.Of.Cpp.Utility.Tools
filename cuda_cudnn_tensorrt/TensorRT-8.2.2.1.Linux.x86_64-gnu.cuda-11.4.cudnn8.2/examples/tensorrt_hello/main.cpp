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

nvinfer1::Weights makeWeights(float* ptr, int64_t size) {
    nvinfer1::Weights w;
    w.count = size;
    w.type = nvinfer1::DataType::kFLOAT;
    w.values = ptr;
    return w;
}

int main() {
    Logger logger;
    auto builder = nvinfer1::createInferBuilder(logger);
    const auto explicitBatch = 1U << static_cast<uint32_t>(nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
    auto network = builder->createNetworkV2(explicitBatch);

    // 构建网络 (代码不变)
    const int inputSize = 3, outputSize = 2;
    float weightVals[] = {1.0f, 2.0f, 0.5f, 0.1f, 0.2f, 0.5f};
    float biasVals[]   = {0.3f, 0.8f};
    auto weights = makeWeights(weightVals, 6);
    auto biases  = makeWeights(biasVals, 2);

    auto input = network->addInput("input", nvinfer1::DataType::kFLOAT, nvinfer1::Dims4{1, inputSize, 1, 1});
    auto fcLayer = network->addFullyConnected(*input, outputSize, weights, biases);
    auto prob = network->addActivation(*fcLayer->getOutput(0), nvinfer1::ActivationType::kSIGMOID);
    network->markOutput(*prob->getOutput(0));

    // 构建引擎 (核心修改部分)
    nvinfer1::IBuilderConfig* config = builder->createBuilderConfig();
    config->setMaxWorkspaceSize(1 << 28);

    // 使用 buildEngineWithConfig 代替 buildEngine
    nvinfer1::ICudaEngine* engine = builder->buildEngineWithConfig(*network, *config);
    if (!engine) {
        std::cerr << "Engine 构建失败！" << std::endl;
        return -1;
    }

    // 序列化并保存引擎
    nvinfer1::IHostMemory* serializedModel = engine->serialize();
    std::ofstream engineFile("simple_engine.engine", std::ios::binary);
    engineFile.write(reinterpret_cast<const char*>(serializedModel->data()), serializedModel->size());
    engineFile.close();

    std::cout << "引擎成功保存为 simple_engine.engine" << std::endl;

    // 清理资源
    serializedModel->destroy();
    delete engine;
    delete config;
    delete network;
    delete builder;
    return 0;
}
