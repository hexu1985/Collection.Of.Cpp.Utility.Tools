#pragma once

#include <chrono>
#include <functional>
#include <memory>

class Timer {
public:
    using Callback = std::function<void ()>;
    using Interval = std::chrono::microseconds;
    struct Impl;

    enum Type {
        once = 1,
        repeat = 2,
    };

    Timer(Callback function, Interval interval, Type type=once); 
    void stop(); 
    bool isRunning(); 

private:
    std::shared_ptr<Impl> pimpl_;
};



