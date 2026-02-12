#include <iostream>
#include <vector>
#include <yaml-cpp/yaml.h>

int main() {
    // 示例YAML字符串
    std::string yaml_string = R"(
# 简单数组
fruits:
  - apple
  - banana
  - orange

# 数字数组
numbers: [1, 2, 3, 4, 5]

# 混合类型数组
mixed:
  - hello
  - 42
  - 3.14
  - true

# 对象数组
people:
  - name: Alice
    age: 25
  - name: Bob
    age: 30
  - name: Charlie
    age: 35
)";

    try {
        YAML::Node config = YAML::Load(yaml_string);
        
        // 方法1: 使用迭代器遍历数组
        std::cout << "Fruits: ";
        const YAML::Node& fruits = config["fruits"];
        for (YAML::const_iterator it = fruits.begin(); it != fruits.end(); ++it) {
            std::cout << it->as<std::string>() << " ";
        }
        std::cout << std::endl;
        
        // 方法2: 使用范围for循环
        std::cout << "Numbers: ";
        for (const auto& num : config["numbers"]) {
            std::cout << num.as<int>() << " ";
        }
        std::cout << std::endl;
        
        // 方法3: 直接转换为vector
        std::vector<int> numbers = config["numbers"].as<std::vector<int>>();
        std::cout << "Numbers as vector: ";
        for (int n : numbers) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
        
        // 方法4: 处理混合类型
        std::cout << "Mixed array:" << std::endl;
        const YAML::Node& mixed = config["mixed"];
        for (std::size_t i = 0; i < mixed.size(); ++i) {
            const YAML::Node& item = mixed[i];
            if (item.IsScalar()) {
                if (item.IsDefined()) {
                    std::cout << "  - " << item.as<std::string>() << std::endl;
                }
            }
        }
        
        // 方法5: 解析对象数组
        std::cout << "People:" << std::endl;
        const YAML::Node& people = config["people"];
        for (const auto& person : people) {
            std::string name = person["name"].as<std::string>();
            int age = person["age"].as<int>();
            std::cout << "  - Name: " << name << ", Age: " << age << std::endl;
        }
        
    } catch (const YAML::Exception& e) {
        std::cerr << "YAML解析错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}