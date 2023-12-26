#pragma once

#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(int interval, Callback function) {
        pimpl = std::make_shared<Impl>(interval, function);
    }

    void start() {
        std::thread t([pimpl=pimpl]() {
            if(!pimpl->active.load()) return;
            std::this_thread::sleep_for(std::chrono::seconds(pimpl->interval));
            if(!pimpl->active.load()) return;
            pimpl->function();
        });
        t.detach();
    }

    void cancel() {
        pimpl->active.store(false);
    }

private:
    struct Impl {
        Impl(int interval_, Callback function_): interval(interval_), function(function_) {}

        int interval;
        Callback function;
        std::atomic<bool> active{true};
    };

private:
    std::shared_ptr<Impl> pimpl;
};

