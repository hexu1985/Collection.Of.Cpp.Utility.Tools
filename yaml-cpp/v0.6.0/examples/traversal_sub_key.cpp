#include <iostream>
#include "yaml-cpp/yaml.h"

int main() {
    // 示例 YAML 内容
    const char* yaml_content = R"(
server:
  host: "localhost"
  port: 8080
  ssl: true

database:
  name: "test_db"
  tables: ["users", "products", "orders"]
  connections:
    max: 20
    min: 5
    timeout: 30

logging:
  level: "info"
  files: ["app.log", "error.log"]
)";

    try {
        YAML::Node config = YAML::Load(yaml_content);
        
        std::cout << "=== 遍历所有顶级节点 ===" << std::endl;
        for (const auto& entry : config) {
            std::string key = entry.first.as<std::string>();
            std::cout << "\n节点: " << key << std::endl;
            
            // 处理不同类型的子节点
            if (entry.second.IsMap()) {
                std::cout << "  子节点数量: " << entry.second.size() << std::endl;
                
                // 遍历二级节点
                for (const auto& subentry : entry.second) {
                    std::cout << "  - " << subentry.first.as<std::string>() << ": ";
                    if (subentry.second.IsScalar()) {
                        std::cout << subentry.second.as<std::string>();
                    }
                    std::cout << std::endl;
                }
            }
        }
        
        std::cout << "\n=== 访问特定路径 ===" << std::endl;
        if (config["database"]["tables"].IsSequence()) {
            std::cout << "数据库表: ";
            for (const auto& table : config["database"]["tables"]) {
                std::cout << table.as<std::string>() << " ";
            }
            std::cout << std::endl;
        }
        
    } catch (const YAML::Exception& e) {
        std::cerr << "解析错误: " << e.what() << std::endl;
    }
    
    return 0;
}