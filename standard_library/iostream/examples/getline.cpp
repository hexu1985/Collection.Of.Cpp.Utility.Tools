#include <iostream>
#include <string>

int main() {
    std::string message;

    std::cout << "请输入一行文本: ";
    std::getline(std::cin, message);

    std::cout << "你输入的内容是: " << message << std::endl;

    return 0;
}
