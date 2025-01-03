#include "frame_delay_checker.hpp"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

const int counter1 = 1;
const int counter2 = 2;

void warning1(milliseconds interval_ms) {
    static system_clock::time_point latest_time{};
    auto now = system_clock::now();
    if (now - latest_time > interval_ms) {
        std::cout << "warning1()\n";
        latest_time = now;
    }
}

void warning2(milliseconds interval_ms) {
    static system_clock::time_point latest_time{};
    auto now = system_clock::now();
    if (now - latest_time > interval_ms) {
        std::cout << "warning2()\n";
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
