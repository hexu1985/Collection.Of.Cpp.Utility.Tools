#include <ctime>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // 转换为本地时间
    std::tm tm;
    // 使用localtime_s在Windows，localtime_r在POSIX，但为了简单，使用localtime并加锁？
    // 或者使用gmtime，因为时区无所谓？但格式没有指定时区，一般用本地时间。
    // 我们可以使用std::localtime，注意它不是线程安全的，但这里只是演示。
    // 更好的做法是使用std::localtime的线程安全版本。
    // 为了跨平台，可以使用以下方式：
#if defined(_WIN32)
    localtime_s(&tm, &in_time_t);
#else
    localtime_r(&in_time_t, &tm);
#endif

    // 或者使用std::put_time，但需要流
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

int main() {
    std::cout << getCurrentTime() << std::endl;
}
