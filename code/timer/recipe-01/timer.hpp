#pragma once

#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(double interval, Callback function) {
        pimpl = std::make_shared<Impl>(interval, function);
    }

    void Start() {
        std::thread t([pimpl=pimpl]() {
            if(!pimpl->active.load()) return;
            std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long int>(pimpl->interval*1000000)));
            if(!pimpl->active.load()) return;
            pimpl->function();
        });
        t.detach();
    }

    void Cancel() {
        pimpl->active.store(false);
    }

private:
    struct Impl {
        Impl(double interval_, Callback function_): interval(interval_), function(function_) {}

        double interval;
        Callback function;
        std::atomic<bool> active{true};
    };

private:
    std::shared_ptr<Impl> pimpl;
};

