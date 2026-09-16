#include <iostream>
#include <string>

int main() {
    // 1. 基本转换
    std::string s1 = "12345";
    int n1 = std::stoi(s1);
    //int n1 = std::stoi("12345");
    std::cout << "n1 = " << n1 << std::endl;  // 12345

    // 2. 带符号
    std::string s2 = "-42";
    int n2 = std::stoi(s2);
    std::cout << "n2 = " << n2 << std::endl;  // -42

    // 3. 跳过前导空白
    std::string s3 = "   100abc";
    int n3 = std::stoi(s3);
    std::cout << "n3 = " << n3 << std::endl;  // 100

    // 4. 获取解析位置
    std::size_t pos;
    std::string s4 = "3.14";
    int n4 = std::stoi(s4, &pos);
    std::cout << "n4 = " << n4 << ", pos = " << pos << std::endl;  // 3, pos = 1

    return 0;
}
