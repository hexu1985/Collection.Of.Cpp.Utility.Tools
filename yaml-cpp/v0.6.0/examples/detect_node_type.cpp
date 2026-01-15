#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>

enum class NodeType {
    NOT_DEFINE,
    NULL_VALUE,
    SUB_NODE,
    STRING,
    INTEGER,
    FLOAT,
    BOOLEAN,
    UNKNOWN,
};

NodeType detectNodeType(const YAML::Node& node) {
    if (!node.IsDefined()) {
        return NodeType::NOT_DEFINE;
    }

    if (node.IsNull()) {
        return NodeType::NULL_VALUE;
    }

    if (node.IsMap()) {
        return NodeType::SUB_NODE;
    }
    
    if (!node.IsScalar()) {
        return NodeType::UNKNOWN;
    }
    
    // 尝试转换为布尔值
    try {
        node.as<bool>();
        return NodeType::BOOLEAN;
    } catch (const YAML::TypedBadConversion<bool>&) {
        // 不是布尔值，继续尝试
    }
    
    // 尝试转换为整数
    try {
        node.as<int>();
        return NodeType::INTEGER;
    } catch (const YAML::TypedBadConversion<int>&) {
        // 不是整数，继续尝试
    }
    
    // 尝试转换为浮点数
    try {
        node.as<double>();
        return NodeType::FLOAT;
    } catch (const YAML::TypedBadConversion<double>&) {
        // 不是浮点数
    }
    
    // 尝试转换为字符串（应该总是成功）
    try {
        node.as<std::string>();
        return NodeType::STRING;
    } catch (...) {
        // 理论上不应该到这里
    }
    
    return NodeType::UNKNOWN;
}

void print_node_type(const YAML::Node& node) {
    NodeType type = detectNodeType(node);
    switch (type) {
        case NodeType::STRING: std::cout << "STRING"; break;
        case NodeType::INTEGER: std::cout << "INTEGER"; break;
        case NodeType::FLOAT: std::cout << "FLOAT"; break;
        case NodeType::BOOLEAN: std::cout << "BOOLEAN"; break;
        case NodeType::NULL_VALUE: std::cout << "NULL"; break;
        case NodeType::SUB_NODE: std::cout << "SUB_NODE"; break;
        case NodeType::NOT_DEFINE: std::cout << "NOT_DEFINE"; break;
        default: std::cout << "UNKNOWN"; break;
    }
    if (type == NodeType::UNKNOWN || type == NodeType::SUB_NODE || type == NodeType::NOT_DEFINE) {
        std::cout << std::endl;
    } else if (type == NodeType::NULL_VALUE) {
        std::cout << " (值: NULL)" << std::endl;
    } else {
        std::cout << " (值: " << node.as<std::string>() << ")" << std::endl;
    }
}

int main() {
    std::string yaml_text = R"(
string_val: "hello"
int_val: 123
float_val: 3.14
bool_val: true
null_val: null
scientific: 1.23e4
hex_val: 0xFF
octal_val: 0777
subnode:
    key1: 12
    key2: 13
)";
    
    YAML::Node config = YAML::Load(yaml_text);
    
    for (const auto& entry : config) {
        std::string key = entry.first.as<std::string>();
        std::cout << key << ": ";
        print_node_type(entry.second);
    }

    std::string key = "not_such_key";
    std::cout << key << ": ";
    print_node_type(config[key]);
    
    return 0;
}
