#include <fstream>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <assert.h>

void save_file(YAML::Node& node) {
    std::ofstream file("test.yaml");
    file << node <<std::endl;
}

int main()
{
    YAML::Node node;  

    save_file(node);
}
