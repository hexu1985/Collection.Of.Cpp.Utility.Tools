/**
 * @file Timer.hpp
 * @brief 一个基于thread的超简单的定时器类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-09-03
 *
 * @see 
 * https://www.fluentcpp.com/2018/12/28/timer-cpp/ \n
 * https://github.com/99xt/timercpp.git
 */
#ifndef MINI_UTIL_TIMER_INC
#define MINI_UTIL_TIMER_INC

#include <atomic>
#include <thread>
#include <chrono>
#include <functional>

namespace mini_util {

class Timer {
public:
    typedef std::function<void ()> Callback;

    template <class Rep, class Period>
    void SetTimeout(Callback function, const std::chrono::duration<Rep, Period> &delay);

    template <class Rep, class Period>
    void SetInterval(Callback function, const std::chrono::duration<Rep, Period> &interval);

    void Stop();
private:
    std::atomic<bool> clear_{false};
};

template <class Rep, class Period>
void Timer::SetTimeout(Callback function, const std::chrono::duration<Rep, Period> &delay) 
{
    this->clear_ = false;
    std::thread t([=]() {
        if(this->clear_) return;
        std::this_thread::sleep_for(delay);
        if(this->clear_) return;
        function();
    });
    t.detach();
}

template <class Rep, class Period>
void Timer::SetInterval(Callback function, const std::chrono::duration<Rep, Period> &interval)
{
    this->clear_ = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear_) return;
            std::this_thread::sleep_for(interval);
            if(this->clear_) return;
            function();
        }
    });
    t.detach();
}

inline
void Timer::Stop() 
{
    this->clear_ = true;
}

}   // namespace mini_util

#endif
