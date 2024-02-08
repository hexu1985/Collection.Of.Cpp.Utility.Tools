#pragma once

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

    template <class Rep, class Period>
    void setTimeout(Callback function, const std::chrono::duration<Rep, Period> &delay)
    {
        start_timer(std::move(function), std::chrono::duration<double>(delay).count(), false);
    }

    template <class Rep, class Period>
    void setInterval(Callback function, const std::chrono::duration<Rep, Period> &interval)
    {
        start_timer(std::move(function), std::chrono::duration<double>(interval).count(), true);
    }

    void stop();

public:
    struct Impl; 

private:
    void start_timer(Callback function, double interval, bool is_period); // unit second

private:
    std::shared_ptr<Impl> pimpl;
};

