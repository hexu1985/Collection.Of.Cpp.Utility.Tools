#pragma once

#include <atomic>
#include <thread>
#include <chrono>
#include <functional>

class Timer {
public:
    typedef std::function<void ()> Callback;

    template <class Rep, class Period>
    void setTimeout(Callback function, const std::chrono::duration<Rep, Period> &delay);

    template <class Rep, class Period>
    void setInterval(Callback function, const std::chrono::duration<Rep, Period> &interval);

    void stop();

private:
    std::atomic<bool> active{true};
};

template <class Rep, class Period>
void Timer::setTimeout(Callback function, const std::chrono::duration<Rep, Period> &delay) {
    active = true;
    std::thread t([=]() {
        if(!active.load()) return;
        std::this_thread::sleep_for(delay);
        if(!active.load()) return;
        function();
    });
    t.detach();
}

template <class Rep, class Period>
void Timer::setInterval(Callback function, const std::chrono::duration<Rep, Period> &interval) {
    active = true;
    std::thread t([=]() {
        while(active.load()) {
            std::this_thread::sleep_for(interval);
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

