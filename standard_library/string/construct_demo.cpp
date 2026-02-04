// string constructor
#include <iostream>
#include <string>
#include <vector>

void printString(const std::string& str) {
    std::cout << "in void printString(const std::string& str)" << std::endl;
    std::cout << "str: " << str << std::endl;
}

void printString(const std::vector<std::string>& str_list) {
    std::cout << "in void printString(const std::vector<std::string>& str_list)" << std::endl;
    std::cout << "str_list: ";
    for (const auto& str: str_list) {
        std::cout << str << " ";
    }
    std::cout << std::endl;
}

void printString(std::initializer_list<std::string> init_list) {
    printString(std::vector<std::string>(init_list));
}

int main() {
    printString("hello");
    printString({"hello", "world"});
    printString({"hello"});
    printString({"hello", "world", "!"});
}
