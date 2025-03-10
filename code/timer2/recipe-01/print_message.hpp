#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

inline
void print_message(const std::string& message) {
    auto now = std::chrono::system_clock::now();

    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch() % std::chrono::seconds(1)
    );

    std::tm now_tm = *std::localtime(&now_time_t);

    // 打印时间，精确到毫秒
    std::cout << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S")
              << '.' << std::setfill('0') << std::setw(6) << now_us.count()
              << ": " << message
              << std::endl;
}
