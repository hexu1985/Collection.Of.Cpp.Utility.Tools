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

    void setTimeout(Callback function, int delay_ms)    // unit millisecond
    {
        start_timer(std::move(function), static_cast<double>(delay_ms/1000), false);
    }

    void setInterval(Callback function, int interval_ms)  // unit millisecond
    {
        start_timer(std::move(function), static_cast<double>(interval_ms/1000), true);
    }

    void stop();

public:
    struct Impl; 

private:
    void start_timer(Callback function, double interval, bool is_period); // unit second

private:
    std::shared_ptr<Impl> pimpl;
};

