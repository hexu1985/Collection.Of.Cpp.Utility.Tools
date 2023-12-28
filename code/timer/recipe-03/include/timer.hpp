#ifndef TIMER_INC
#define TIMER_INC

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

class Timer {
public:
    typedef std::function<void ()> Callback;
    typedef std::chrono::system_clock Clock;
    typedef Clock::time_point TimePoint;

    Timer();
    ~Timer();

    void setTimeout(Callback function, int delay_ms);
    void setInterval(Callback function, int interval_ms);
    void stop();

private:
    friend class TimerThread;

private:
    Callback function;      // 超时回调函数
    int interval = -1;      // > 0是周期性的, <= 0是一次性的, 单位ms
    TimePoint timeout;      // 下次超时时间
};

#endif
