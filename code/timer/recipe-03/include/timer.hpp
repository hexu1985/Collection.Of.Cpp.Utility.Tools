#ifndef MINI_UTIL_TIMER_INC
#define MINI_UTIL_TIMER_INC

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

namespace mini_util {

class TimerThread;

class Timer {
public:
    typedef std::function<void ()> Callback;
    typedef std::chrono::system_clock Clock;
    typedef Clock::time_point TimePoint;

    Timer();
    ~Timer();

    void SetTimeout(Callback function, int delay);
    void SetInterval(Callback function, int interval);
    void Stop();

private:
    friend class TimerThread;

private:
    Callback function_;
    int interval_ = -1;     // > 0是周期性的, <= 0是一次性的, 单位ms
    TimePoint timeout_;     // 下次超时时间
};

}   // namespace mini_util

#endif
