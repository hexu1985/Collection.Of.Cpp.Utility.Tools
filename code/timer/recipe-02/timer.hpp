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

    void setTimeout(Callback function, int delay);      // unit millisecond
    void setInterval(Callback function, int interval);  // unit millisecond
    void stop();

public:
    struct Impl; 

private:
    std::shared_ptr<Impl> pimpl;
};

