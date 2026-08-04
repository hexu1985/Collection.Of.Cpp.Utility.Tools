#pragma once
#include <string>
#include <cstdint>
#include <fastcdr/Cdr.h>

struct SensorData {
    uint32_t id;
    float temperature;
    double humidity;
    uint64_t timestamp;
    
    // 序列化方法
    void serialize(eprosima::fastcdr::Cdr& cdr) const {
        cdr << id << temperature << humidity << timestamp;
    }
    
    // 反序列化方法
    void deserialize(eprosima::fastcdr::Cdr& cdr) {
        cdr >> id >> temperature >> humidity >> timestamp;
    }
};

// 工具函数：序列化到 std::string
std::string serializeToString(const SensorData& data);

// 工具函数：从 std::string 反序列化
SensorData deserializeFromString(const std::string& data);

// 工具函数：序列化到 vector<uint8_t>
std::vector<uint8_t> serializeToVector(const SensorData& data);
