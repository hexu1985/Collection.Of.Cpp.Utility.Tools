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
#ifndef MINI_UTILS_TIMER_INC
#define MINI_UTILS_TIMER_INC

#include <atomic>
#include <thread>
#include <chrono>
#include <functional>

namespace mini_utils {

/**
 * @brief 一个基于thread的超简单的定时器类
 */
class Timer {
public:
    /**
     * @brief 定时器到时回调函数
     */
    typedef std::function<void ()> Callback;

    /**
     * @brief 设置一次性定时器
     *
     * @tparam Rep std::duration<Rep, Period>中的Rep(Representation)
     * @tparam Period std::duration<Rep, Period>中的Period
     * @param function 定时器到期回调函数
     * @param delay 定时器到期时间, 相对时间
     */
    template <class Rep, class Period>
    void setTimeoutFor(Callback function, const std::chrono::duration<Rep, Period> &delay);

    /**
     * @brief 设置一次性定时器
     *
     * @tparam Rep std::duration<Rep, Period>中的Rep(Representation)
     * @tparam Period std::duration<Rep, Period>中的Period
     * @param function 定时器到期回调函数
     * @param delay 定时器到期时间, 绝对时间
     */
    template <class Clock, class Duration>
    void setTimeoutUntil(Callback function, const std::chrono::time_point<Clock, Duration> &delay);

    /**
     * @brief 设置周期性定时器
     *
     * @tparam Rep std::duration<Rep, Period>中的Rep(Representation)
     * @tparam Period std::duration<Rep, Period>中的Period
     * @param function 定时器到期回调函数
     * @param interval 定时器超时周期
     */
    template <class Rep, class Period>
    void setInterval(Callback function, const std::chrono::duration<Rep, Period> &interval);

    /**
     * @brief 停止定时器
     */
    void stop();

    Timer() = default; 
    ~Timer() = default; 

private:
    std::atomic<bool> clear_{false};
};

template <class Rep, class Period>
void Timer::setTimeoutFor(Callback function, const std::chrono::duration<Rep, Period> &delay) 
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

template <class Clock, class Duration>
void Timer::setTimeoutUntil(Callback function, const std::chrono::time_point<Clock, Duration> &delay)
{
    this->clear_ = false;
    std::thread t([=]() {
        if(this->clear_) return;
        std::this_thread::sleep_until(delay);
        if(this->clear_) return;
        function();
    });
    t.detach();
}

template <class Rep, class Period>
void Timer::setInterval(Callback function, const std::chrono::duration<Rep, Period> &interval)
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
void Timer::stop() 
{
    this->clear_ = true;
}

}    // namespace mini_utils

#endif
