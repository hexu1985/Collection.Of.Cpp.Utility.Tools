#include "frame_delay_checker.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

const int counter1 = 1;
const int counter2 = 2;

void print_message(const std::string& message) {
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
              << ": " << message
              << std::endl;
}

void warning1(milliseconds interval_ms) {
    static system_clock::time_point latest_time{};
    auto now = system_clock::now();
    if (now - latest_time > interval_ms) {
        print_message("warning1()");
        latest_time = now;
    }
}

void warning2(milliseconds interval_ms) {
    static system_clock::time_point latest_time{};
    auto now = system_clock::now();
    if (now - latest_time > interval_ms) {
        print_message("warning2()");
        latest_time = now;
    }
}

void frame_update1(FrameDelayChecker& checker) {
    for (int i = 0; i < 30; i++) {
        checker.IncreaseFrameCount(counter1);
        sleep_for(milliseconds{20});
    }
}

void frame_update2(FrameDelayChecker& checker) {
    sleep_for(milliseconds{30});
    checker.IncreaseFrameCount(counter2);
    sleep_for(milliseconds{150});
    checker.IncreaseFrameCount(counter2);
    sleep_for(milliseconds{150});
    checker.IncreaseFrameCount(counter2);
}

int main() {
    FrameDelayChecker checker{milliseconds{10}};
    {
        FrameDelayChecker::CheckConfig config;
        config.expired_threshold_ms = milliseconds{50};
        config.expired_callback = []() { warning1(milliseconds{50}); };
        if (!checker.RegisterFrameDelayCheck(counter1, config)) {
            std::cout << "RegisterFrameDelayCheck failed!\n";
            return -1;
        }
    }

    {
        FrameDelayChecker::CheckConfig config;
        config.expired_threshold_ms = milliseconds{100};
        config.expired_callback = []() { warning2(milliseconds{100}); };
        if (!checker.RegisterFrameDelayCheck(counter2, config)) {
            std::cout << "RegisterFrameDelayCheck failed!\n";
            return -1;
        }
    }

    checker.Start();
    std::thread thread1{frame_update1, std::ref(checker)};
    std::thread thread2{frame_update2, std::ref(checker)};

    thread1.join();
    thread2.join();
    checker.Stop();

    return 0;
}
