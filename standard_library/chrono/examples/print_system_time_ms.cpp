#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

int main() {
    // 获取当前系统时间
    auto now = std::chrono::system_clock::now();

    // 将时间转换为 time_t，以便使用 C 标准库函数进行格式化
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    // 将 time_t 转换为 tm 结构体，以便获取年、月、日、时、分、秒等信息
    std::tm now_tm = *std::localtime(&now_time_t);

    // 获取毫秒部分
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    // 打印时间，精确到毫秒
    std::cout << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S")
              << '.' << std::setfill('0') << std::setw(3) << milliseconds.count()
              << std::endl;

    return 0;
}
