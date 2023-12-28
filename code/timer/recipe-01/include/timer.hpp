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

    void setTimeout(Callback function, int delay);
    void setInterval(Callback function, int interval);
    void stop();

private:
    std::atomic<bool> active{true};
};

inline 
void Timer::setTimeout(Callback function, int delay) {
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
void Timer::setInterval(Callback function, int interval) {
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
void Timer::stop() {
    active = false;
}

#endif
