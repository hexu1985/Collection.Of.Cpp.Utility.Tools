#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>

// 您的 ParticipantId 结构体
struct ParticipantId {
    uint32_t domain_id;
    std::string participant_name;

    friend bool operator ==(const ParticipantId& lhs, const ParticipantId& rhs) {
        return lhs.domain_id == rhs.domain_id &&
            lhs.participant_name == rhs.participant_name;
    }
};

// 手动实现（不依赖 Boost）
template <typename T>
inline void hash_combine(std::size_t& seed, const T& v) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// 自定义哈希函数（必须为 std::unordered_map 提供）
struct ParticipantIdHash {
    std::size_t operator()(const ParticipantId& id) const {
        size_t seed = 0;
        hash_combine(seed, id.domain_id);
        hash_combine(seed, id.participant_name);
        return seed;
    }
};

// 为了便于输出，重载 operator<<
std::ostream& operator<<(std::ostream& os, const ParticipantId& id) {
    os << "[" << id.domain_id << ", " << id.participant_name << "]";
    return os;
}

// 测试函数
void testUnorderedMap() {
    // 使用自定义哈希和相等比较器
    std::unordered_map<ParticipantId, std::string, ParticipantIdHash> m_part_map;
    
    std::cout << "===== 测试1: 插入元素 =====" << std::endl;
    
    // 创建几个 ParticipantId 对象
    ParticipantId id1{100, "Alice"};
    ParticipantId id2{100, "Bob"};
    ParticipantId id3{200, "Alice"};
    ParticipantId id4{200, "Bob"};
    ParticipantId id5{100, "Alice"};  // 与 id1 相同
    
    // 插入元素
    m_part_map[id1] = "Participant_001";
    m_part_map[id2] = "Participant_002";
    m_part_map[id3] = "Participant_003";
    m_part_map[id4] = "Participant_004";
    
    std::cout << "插入4个元素后的map大小: " << m_part_map.size() << std::endl;
    
    // 尝试插入重复键（应该覆盖现有值）
    m_part_map[id5] = "Participant_001_Updated";
    std::cout << "插入重复键后的map大小: " << m_part_map.size() << " (应为4)" << std::endl;
    
    std::cout << "\n===== 测试2: 查找元素 =====" << std::endl;
    
    // 查找存在的元素
    auto it = m_part_map.find(id1);
    if (it != m_part_map.end()) {
        std::cout << "查找 id1 " << id1 << " -> " << it->second << std::endl;
    }
    
    // 查找不存在的元素
    ParticipantId id6{300, "Charlie"};
    it = m_part_map.find(id6);
    if (it == m_part_map.end()) {
        std::cout << "查找不存在的 id6 " << id6 << " -> 未找到" << std::endl;
    }
    
    std::cout << "\n===== 测试3: 遍历所有元素 =====" << std::endl;
    for (const auto& pair : m_part_map) {
        std::cout << "键: " << pair.first << ", 值: " << pair.second << std::endl;
    }
    
    std::cout << "\n===== 测试4: 删除元素 =====" << std::endl;
    
    // 按键删除
    size_t erased = m_part_map.erase(id2);
    std::cout << "删除 id2, 删除了 " << erased << " 个元素" << std::endl;
    std::cout << "删除后map大小: " << m_part_map.size() << std::endl;
    
    // 按迭代器删除
    auto it2 = m_part_map.find(id3);
    if (it2 != m_part_map.end()) {
        m_part_map.erase(it2);
        std::cout << "删除 id3 后map大小: " << m_part_map.size() << std::endl;
    }
    
    std::cout << "\n删除后剩余元素:" << std::endl;
    for (const auto& pair : m_part_map) {
        std::cout << "键: " << pair.first << ", 值: " << pair.second << std::endl;
    }
    
    std::cout << "\n===== 测试5: 使用 operator[] =====" << std::endl;
    
    // 使用 operator[] 访问（如果键不存在，会插入默认值）
    std::cout << "访问不存在的键 id6: " << m_part_map[id6] << " (默认空字符串)" << std::endl;
    std::cout << "访问后map大小: " << m_part_map.size() << std::endl;
    
    // 使用 operator[] 修改值
    m_part_map[id6] = "Participant_006";
    std::cout << "修改后 id6 的值: " << m_part_map[id6] << std::endl;
    
    std::cout << "\n===== 测试6: at() 方法 =====" << std::endl;
    try {
        std::string value = m_part_map.at(id1);
        std::cout << "at() 访问 id1: " << value << std::endl;
        
        ParticipantId id7{999, "Unknown"};
        value = m_part_map.at(id7);  // 这会抛出异常
    } catch (const std::out_of_range& e) {
        std::cout << "at() 访问不存在的键抛出异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n===== 测试7: 清空map =====" << std::endl;
    m_part_map.clear();
    std::cout << "清空后map大小: " << m_part_map.size() << std::endl;
    std::cout << "map是否为空: " << (m_part_map.empty() ? "是" : "否") << std::endl;
    
    std::cout << "\n===== 测试8: 性能测试（插入10000个元素） =====" << std::endl;
    for (int i = 0; i < 10000; ++i) {
        ParticipantId id{static_cast<uint32_t>(i), "Participant_" + std::to_string(i)};
        m_part_map[id] = "Value_" + std::to_string(i);
    }
    std::cout << "插入10000个元素后map大小: " << m_part_map.size() << std::endl;
    
    // 查找一个存在的元素
    ParticipantId search_id{5000, "Participant_5000"};
    auto search_it = m_part_map.find(search_id);
    if (search_it != m_part_map.end()) {
        std::cout << "找到元素: " << search_it->second << std::endl;
    }
    
    std::cout << "\n===== 所有测试完成 =====" << std::endl;
}

int main() {
    testUnorderedMap();
    return 0;
}
