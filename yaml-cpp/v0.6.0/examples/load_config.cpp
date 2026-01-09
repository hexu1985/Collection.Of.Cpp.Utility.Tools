#include <iostream>
#include <yaml-cpp/yaml.h>  /* yaml-cpp header file */

int main() {
    /* load yaml file */
    YAML::Node config = YAML::LoadFile("config.yaml");

    /* key-value */
    std::string name = config["name"].as<std::string>();
    int age = config["age"].as<int>();
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl << std::endl;

    /* list */
    YAML::Node fruits = config["fruits"];
    std::cout << "fruits: " << std::endl;
    for (const auto& fruit : fruits) {
        std::cout << "- " << fruit << std::endl;
    }
    std::cout << std::endl;

    /* nested structure */
    YAML::Node person = config["person"];
    std::cout << "person info:" << std::endl;
    std::string p_name = config["person"]["name"].as<std::string>();
    int p_age = config["person"]["age"].as<int>();
    std::cout << "    name: " << p_name << std::endl;
    std::cout << "    age: " << p_age << std::endl;

    YAML::Node hobbies = config["person"]["hobbies"];
    std::cout << "    hobbies:" << std::endl;
    for (const auto& hobby : hobbies) {
        std::cout << "        - " << hobby << std::endl;
    }
    std::cout << std::endl;

    /* multi-line string */
    std::string description = config["description"].as<std::string>();
    std::string summary = config["summary"].as<std::string>();
    std::cout << description << std::endl;
    std::cout << summary << std::endl << std::endl;

    /* data type */
    std::cout << "int: " << config["int_v"].as<int>() << std::endl;
    std::cout << "float: " << config["float_v"].as<float>() << std::endl;
    std::cout << "bool: " << config["bool_v"].as<bool>() << std::endl;
    std::cout << "string: " << config["string_v"].as<std::string>() << std::endl;

    return 0;
}

