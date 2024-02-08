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
        set_timeout_aux(std::move(function), static_cast<double>(delay_ms/1000));
    }

    void setInterval(Callback function, int interval_ms)  // unit millisecond
    {
        set_timeout_aux(std::move(function), static_cast<double>(interval_ms/1000));
    }

    void stop();

public:
    struct Impl; 

private:
    void set_timeout_aux(Callback function, double delay_sec);      // unit second
    void set_interval_aux(Callback function, double interval_sec);  // unit second

private:
    std::shared_ptr<Impl> pimpl;
};

