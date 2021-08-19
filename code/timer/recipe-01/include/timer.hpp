#ifndef MINI_UTIL_TIMER_INC
#define MINI_UTIL_TIMER_INC

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

class Timer {
public:
    typedef std::function<void ()> Callback;

    void SetTimeout(Callback function, int delay);
    void SetInterval(Callback function, int interval);
    void Stop();

private:
    std::atomic<bool> active{true};
};

inline 
void Timer::SetTimeout(Callback function, int delay) {
    active = true;
    std::thread t([=]() {
        if(!active.load()) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(!active.load()) return;
        function();
    });
    t.detach();
}

inline 
void Timer::SetInterval(Callback function, int interval) {
    active = true;
    std::thread t([=]() {
        while(active.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(!active.load()) return;
            function();
        }
    });
    t.detach();
}

inline 
void Timer::Stop() {
    active = false;
}

#endif
