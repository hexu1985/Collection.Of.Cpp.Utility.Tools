#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

static std::string getCurrentTime() {
    using namespace std::chrono;

    // 获取当前时间点
    auto now = system_clock::now();

    // 提取毫秒部分（0-999）
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // 转换为秒级时间并转为本地时间
    std::time_t t = system_clock::to_time_t(now);
    std::tm* bt = std::localtime(&t);

    // 格式化输出
    std::ostringstream oss;
    oss << std::put_time(bt, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}
