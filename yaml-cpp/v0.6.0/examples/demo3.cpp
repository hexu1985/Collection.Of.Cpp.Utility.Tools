#include <fstream>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <assert.h>

int main()
{
    std::ifstream file("test.yaml");
    YAML::Node node = YAML::Load(file);//读取来自test.yaml的node文件
    std::cout << node <<std::endl;

    std::cout << "\n=============================\n";
    YAML::Node node_2 = YAML::LoadFile("test.yaml");//也可以这样读取文件
    std::cout << node_2["node_2"] <<std::endl;//可以直接用下标访问
    for(auto it = node_2.begin(); it != node_2.end(); it++)
        std::cout << it->first << it->second << std::endl;//也可以用迭代器访问
}
