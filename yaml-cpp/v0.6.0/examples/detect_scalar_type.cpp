#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>

enum class ScalarType {
    UNKNOWN,
    STRING,
    INTEGER,
    FLOAT,
    BOOLEAN,
    NULL_VALUE
};

ScalarType detectScalarType(const YAML::Node& node) {
    if (node.IsNull()) {
        return ScalarType::NULL_VALUE;
    }
    
    if (!node.IsScalar()) {
        return ScalarType::UNKNOWN;
    }
    
    // 尝试转换为布尔值
    try {
        node.as<bool>();
        return ScalarType::BOOLEAN;
    } catch (const YAML::TypedBadConversion<bool>&) {
        // 不是布尔值，继续尝试
    }
    
    // 尝试转换为整数
    try {
        node.as<int>();
        return ScalarType::INTEGER;
    } catch (const YAML::TypedBadConversion<int>&) {
        // 不是整数，继续尝试
    }
    
    // 尝试转换为浮点数
    try {
        node.as<double>();
        return ScalarType::FLOAT;
    } catch (const YAML::TypedBadConversion<double>&) {
        // 不是浮点数
    }
    
    // 尝试转换为字符串（应该总是成功）
    try {
        node.as<std::string>();
        return ScalarType::STRING;
    } catch (...) {
        // 理论上不应该到这里
    }
    
    return ScalarType::UNKNOWN;
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
)";
    
    YAML::Node config = YAML::Load(yaml_text);
    
    for (const auto& entry : config) {
        std::string key = entry.first.as<std::string>();
        ScalarType type = detectScalarType(entry.second);
        
        std::cout << key << ": ";
        switch (type) {
            case ScalarType::STRING: std::cout << "STRING"; break;
            case ScalarType::INTEGER: std::cout << "INTEGER"; break;
            case ScalarType::FLOAT: std::cout << "FLOAT"; break;
            case ScalarType::BOOLEAN: std::cout << "BOOLEAN"; break;
            case ScalarType::NULL_VALUE: std::cout << "NULL"; break;
            default: std::cout << "UNKNOWN"; break;
        }
        if (type == ScalarType::UNKNOWN) {
            std::cout << std::endl;
        } else if (type == ScalarType::NULL_VALUE) {
            std::cout << " (值: NULL)" << std::endl;
        } else {
            std::cout << " (值: " << entry.second.as<std::string>() << ")" << std::endl;
        }
    }
    
    return 0;
}
