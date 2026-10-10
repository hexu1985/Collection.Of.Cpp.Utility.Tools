#include <iostream>
#include <string>

int main() {
    std::string message;

    std::cout << "请输入多行文本（按 Ctrl+D 结束，Windows 下按 Ctrl+Z 再回车）:\n";

    // getline 在遇到 EOF 时返回的流会变为 false，循环自动结束
    while (std::getline(std::cin, message)) {
        std::cout << "读到: " << message << std::endl;
    }

    std::cout << "检测到 EOF，循环结束。" << std::endl;

    return 0;
}
