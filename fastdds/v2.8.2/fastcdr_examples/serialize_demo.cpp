#include "serialize_demo.hpp"
#include <fastcdr/FastBuffer.h>
#include <vector>
#include <cstring>

std::string serializeToString(const SensorData& data) {
    // 预分配足够大的缓冲区（根据实际数据调整大小）
    char buffer[1024];
    eprosima::fastcdr::FastBuffer fast_buffer(buffer, sizeof(buffer));
    eprosima::fastcdr::Cdr cdr(fast_buffer);
    
    // 序列化
    data.serialize(cdr);
    
    // 转换为 string
    return std::string(
        reinterpret_cast<char*>(fast_buffer.getBuffer()),
        fast_buffer.getBufferSize()
    );
}

SensorData deserializeFromString(const std::string& data) {
    // 注意：这里需要去掉 const，因为 FastBuffer 需要可修改的缓冲区
    // 实际使用时建议复制一份数据
    std::vector<char> buffer(data.begin(), data.end());
    
    eprosima::fastcdr::FastBuffer fast_buffer(buffer.data(), buffer.size());
    eprosima::fastcdr::Cdr cdr(fast_buffer);
    
    SensorData result;
    result.deserialize(cdr);
    return result;
}

std::vector<uint8_t> serializeToVector(const SensorData& data) {
    char buffer[1024];
    eprosima::fastcdr::FastBuffer fast_buffer(buffer, sizeof(buffer));
    eprosima::fastcdr::Cdr cdr(fast_buffer);
    
    data.serialize(cdr);
    
    auto* ptr = reinterpret_cast<uint8_t*>(fast_buffer.getBuffer());
    return std::vector<uint8_t>(ptr, ptr + fast_buffer.getBufferSize());
}
