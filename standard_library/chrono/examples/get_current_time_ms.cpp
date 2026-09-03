#include <iostream>
#include <chrono>
#include <cstdint>
#include <ctime>

// 获取当前毫秒时间戳
inline uint64_t get_current_time_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

// 将毫秒时间戳转换为可读时间（用于调试）
inline std::string ms_to_string(uint64_t ms) {
    std::chrono::milliseconds duration(ms);
    std::chrono::system_clock::time_point tp(duration);
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm* tm = std::gmtime(&time);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    int millis = ms % 1000;
    return std::string(buffer) + "." + std::to_string(millis);
}

int main() {
    auto ms = get_current_time_ms();
    std::cout << "now timestamp_ms: " << ms << std::endl;
    std::cout << "now time: " << ms_to_string(ms) << std::endl;

    return 0;
}
