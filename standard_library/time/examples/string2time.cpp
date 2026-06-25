#define _XOPEN_SOURCE      // 启用 strptime 声明（glibc 需要）
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>

bool string2time(const std::string& str, time_t& t) {
    struct tm tm{};
    char* result = nullptr;
    static const std::vector<const char*> format_list{
        "%Y-%m-%d,%H-%M-%S",
        "%Y-%m-%d_%H-%M-%S"
    };

    for (const char* format: format_list) {
        result = strptime(str.c_str(), format, &tm);
        if (result == nullptr) {
            continue;
        }
        t = mktime(&tm);
        return true;
    }
    return false;
}

void test_string2time(const std::string& str) {
    time_t t;
    if (string2time(str, t)) {
        std::cout << "string2time success, origin str: " << str << ", time: " << ctime(&t) << std::endl;
    } else {
        std::cout << "string2time failed, origin str: " << str << std::endl;
    }
}

int main() {
    test_string2time("2026-06-17,14-30-00");
    test_string2time("2026-06-17_14-30-00");
    test_string2time("2026-06-17_14:30:00");
    test_string2time("2026-06-17,14:30:00");
    return 0;
}
