/**
 * @file timer.hpp
 * @brief 一个基于thread的超简单的定时器类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-09-03
 *
 * @see 
 * https://www.fluentcpp.com/2018/12/28/timer-cpp/ \n
 * https://github.com/99xt/timercpp.git
 */
#ifndef TIMER_INC
#define TIMER_INC

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

#endif
