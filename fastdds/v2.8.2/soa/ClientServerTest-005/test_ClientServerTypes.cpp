#include "ClientServerTypes.hpp"

#include <iostream>
#include <iomanip>

using namespace clientserver; 

void test_Operation() {
    // 创建测试数据
    Operation original;

    original.m_operationType = Operation::MULTIPLICATION;
    original.m_num1 = 25;
    original.m_num2 = 65;
    
    // 序列化到 vector
    std::vector<uint8_t> serialized;
    bool ret;
    ret = original.SerializeToVector(serialized);
    if (ret) {
        std::cout << "SerializeToVector success!" << std::endl;
    } else {
        std::cout << "SerializeToVector failed!" << std::endl;
    }
    std::cout << "Serialized size: " << serialized.size() << " bytes\n";
    
    // 打印十六进制内容
    std::cout << "Hex dump: ";
    for (unsigned char c : serialized) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << "\n\n";
    
    // 反序列化
    Operation recovered;
    ret = recovered.DeserializeFromVector(serialized);
    if (ret) {
        std::cout << "DeserializeFromVector success!" << std::endl;
    } else {
        std::cout << "DeserializeFromVector failed!" << std::endl;
    }
    
    // 验证结果
    std::cout << "Original:  m_operationType=" << (uint32_t) original.m_operationType 
              << ", m_num1=" << original.m_num1
              << ", m_num2=" << original.m_num2 << "\n";
              
    std::cout << "Recovered:  m_operationType=" << (uint32_t) recovered.m_operationType 
              << ", m_num1=" << recovered.m_num1
              << ", m_num2=" << recovered.m_num2 << "\n";
    
    // 检查是否一致
    bool match = (original.m_operationType == recovered.m_operationType) &&
                 (original.m_num1 == recovered.m_num1) &&
                 (original.m_num2 == recovered.m_num2); 
    
    std::cout << "\nSerialization roundtrip: " 
              << (match ? "SUCCESS" : "FAILED") << "\n";
}

void test_Result() {
    // 创建测试数据
    Result original;

    original.m_result = 100;
    
    // 序列化到 vector
    std::vector<uint8_t> serialized;
    bool ret;
    ret = original.SerializeToVector(serialized);
    if (ret) {
        std::cout << "SerializeToVector success!" << std::endl;
    } else {
        std::cout << "SerializeToVector failed!" << std::endl;
    }
    std::cout << "Serialized size: " << serialized.size() << " bytes\n";
    
    // 打印十六进制内容
    std::cout << "Hex dump: ";
    for (unsigned char c : serialized) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << "\n\n";
    
    // 反序列化
    Result recovered;
    ret = recovered.DeserializeFromVector(serialized);
    if (ret) {
        std::cout << "DeserializeFromVector success!" << std::endl;
    } else {
        std::cout << "DeserializeFromVector failed!" << std::endl;
    }
    
    // 验证结果
    std::cout << "Original:  m_result=" << original.m_result << "\n";
              
    std::cout << "Recovered:  m_result=" << recovered.m_result << "\n";
    
    // 检查是否一致
    bool match = (original.m_result == recovered.m_result);
    
    std::cout << "\nSerialization roundtrip: " 
              << (match ? "SUCCESS" : "FAILED") << "\n";
}

int main() {
    test_Operation();
    test_Result();
}
