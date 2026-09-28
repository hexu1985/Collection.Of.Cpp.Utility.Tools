#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

class SystemInfo {
public:
    static std::string get_timestamp(const char* format);
};

std::string SystemInfo::get_timestamp(
        const char* format)
{
    std::stringstream stream;
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::chrono::system_clock::duration tp = now.time_since_epoch();
    tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);
    auto ms = static_cast<unsigned>(tp / std::chrono::milliseconds(1));

#if defined(_WIN32)
    struct tm timeinfo;
    localtime_s(&timeinfo, &now_c);
    //#elif defined(__clang__) && !defined(std::put_time) // TODO arm64 doesn't seem to support std::put_time
    //    (void)now_c;
    //    (void)ms;
#elif (_POSIX_C_SOURCE >= 1) || defined(_XOPEN_SOURCE) || defined(_BSD_SOURCE) || defined(_SVID_SOURCE) || \
    defined(_POSIX_SOURCE) || defined(__unix__)
    std::tm timeinfo;
    localtime_r(&now_c, &timeinfo);
#else
    std::tm timeinfo = *localtime(&now_c);
#endif // if defined(_WIN32)
    char buffer[256];
    std::strftime(buffer, sizeof(buffer), format, &timeinfo);
    stream << buffer << "." << std::setw(3) << std::setfill('0') << ms;
    return stream.str();
}

// 可选：提供一个 main 用于测试
#include <iostream>
int main() {
    std::cout << SystemInfo::get_timestamp("%Y-%m-%d %H:%M:%S") << std::endl;
    return 0;
}
