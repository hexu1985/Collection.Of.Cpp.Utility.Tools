#include <fstream>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <assert.h>

int main()
{
    YAML::Node node1 = YAML::Load("{key: value}");
    YAML::Node node2 = node1;  // 浅拷贝，共享底层数据

    std::cout << "node1: " << node1 << std::endl;
    std::cout << "node2: " << node1 << std::endl;

    node2["key"] = "new value";  // 直接修改共享数据
                                 // 此时 node1["key"] 的值也会变为 "new value"
    std::cout << "after modify node2\n";
    std::cout << "node1: " << node1 << std::endl;
    std::cout << "node2: " << node1 << std::endl;

    return 0;
}
