#include <yaml-cpp/yaml.h>
#include <iostream>

int main() {
    YAML::Node config = YAML::Load(R"(
# 值数组（标量数组）
fruits:
  - apple
  - banana
  - orange

# 对象数组（映射数组）
people:
)");

    YAML::Node fruits = config["fruits"];
    YAML::Node people = config["people"];
    
    // 1.1 检查节点类型
    std::cout << "=== 类型检查 ===" << std::endl;
    
    if (fruits.IsSequence()) {
        std::cout << "fruits 是一个序列（数组）" << std::endl;
    }
    
    if (people.IsSequence() || people.IsNull()) {
        std::cout << "people 是一个序列（数组）" << std::endl;
    }
    
    // 1.2 检查数组元素的类型
    std::cout << "\n=== 数组元素类型检查 ===" << std::endl;
    
    // fruits 数组的元素类型
    if (fruits.size() > 0 && fruits[0].IsScalar()) {
        std::cout << "fruits[0] 是标量（值）" << std::endl;
    }
    
    // people 数组的元素类型
    if (people.size() > 0 && people[0].IsMap()) {
        std::cout << "people[0] 是映射（对象）" << std::endl;
    }
    
    // 1.3 使用 Type() 方法
    std::cout << "\n=== 详细类型信息 ===" << std::endl;
    std::cout << "fruits[0] 类型: " << fruits[0].Type() << std::endl;
    std::cout << "people[0] 类型: " << people[0].Type() << std::endl;
    
    // 类型枚举值
    std::cout << "\nYAML::NodeType 枚举值:" << std::endl;
    std::cout << "Undefined: " << YAML::NodeType::Undefined << std::endl;
    std::cout << "Null: " << YAML::NodeType::Null << std::endl;
    std::cout << "Scalar: " << YAML::NodeType::Scalar << std::endl;
    std::cout << "Sequence: " << YAML::NodeType::Sequence << std::endl;
    std::cout << "Map: " << YAML::NodeType::Map << std::endl;
    
    return 0;
}

