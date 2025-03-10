#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

int main() {
    // 获取当前系统时间点
    auto now = std::chrono::system_clock::now();

    // 将时间点转换为时间戳（秒 + 微秒）
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch() % std::chrono::seconds(1)
    );

    // 将时间戳转换为本地时间
    std::tm now_tm = *std::localtime(&now_time_t);

    // 打印时间，精度为微秒
    std::cout << "Current time: "
              << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") 
              << "." << std::setfill('0') << std::setw(6) << now_us.count()
              << std::endl;

    return 0;
}
