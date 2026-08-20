#include "serialize_demo.hpp"
#include <iostream>
#include <iomanip>

int main() {
    // 创建测试数据
    SensorData original;
    original.id = 12345;
    original.temperature = 25.6f;
    original.humidity = 65.2;
    original.timestamp = 1640995200000;
    
    // 序列化到 string
    std::string serialized = serializeToString(original);
    std::cout << "Serialized size: " << serialized.size() << " bytes\n";
    
    // 打印十六进制内容
    std::cout << "Hex dump: ";
    for (unsigned char c : serialized) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << "\n\n";
    
    // 反序列化
    SensorData recovered = deserializeFromString(serialized);
    
    // 验证结果
    std::cout << "Original:  id=" << original.id 
              << ", temp=" << original.temperature
              << ", humidity=" << original.humidity
              << ", timestamp=" << original.timestamp << "\n";
              
    std::cout << "Recovered: id=" << recovered.id
              << ", temp=" << recovered.temperature
              << ", humidity=" << recovered.humidity
              << ", timestamp=" << recovered.timestamp << "\n";
    
    // 检查是否一致
    bool match = (original.id == recovered.id) &&
                 (original.temperature == recovered.temperature) &&
                 (original.humidity == recovered.humidity) &&
                 (original.timestamp == recovered.timestamp);
    
    std::cout << "\nSerialization roundtrip: " 
              << (match ? "SUCCESS" : "FAILED") << "\n";
    
    return match ? 0 : 1;
}
