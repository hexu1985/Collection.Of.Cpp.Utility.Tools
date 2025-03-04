#include <iostream>
#include <string>
#include <absl/crc/crc32c.h>

int main() {
    // 输入数据
    std::string data = "Hello, Abseil CRC!";

    // 计算 CRC32
    absl::crc32c_t crc = absl::ComputeCrc32c(data);

    // 输出结果
    std::cout << "Data: " << data << std::endl;
    std::cout << "CRC32: " << crc << std::endl;

    return 0;
}
