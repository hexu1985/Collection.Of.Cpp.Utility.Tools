#include <yaml-cpp/yaml.h>
#include <iostream>

int main() {
    // 测试 null 值
    YAML::Node null_node = YAML::Load("null");
    
    std::cout << "null_node.IsNull(): " << null_node.IsNull() << std::endl;
    std::cout << "null_node.IsScalar(): " << null_node.IsScalar() << std::endl;
    
    try {
        // 尝试转换为字符串
        std::string str_value = null_node.as<std::string>();
        std::cout << "as<std::string>() 结果: \"" << str_value << "\"" << std::endl;
        std::cout << "字符串长度: " << str_value.length() << std::endl;
    } catch (const YAML::Exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
    
    return 0;
}
