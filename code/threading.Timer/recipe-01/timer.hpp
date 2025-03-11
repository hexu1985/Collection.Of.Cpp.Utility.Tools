#pragma once

#include <thread>
#include <chrono>
#include <atomic>
#include <functional>
#include <memory>

class Timer {
public:
    using Callback = std::function<void ()>;
    using Interval = std::chrono::microseconds;

    enum Type {
        once = 1,
        repeat = 2,
    };

    Timer(Callback function, Interval interval, Type type=once) {
        pimpl = std::make_shared<Impl>();
        pimpl->function = function;
        pimpl->interval = interval;
        pimpl->type = type;
        pimpl->active.store(false);
    }

    void start() {
        pimpl->active.store(true);
        std::thread t([pimpl=pimpl]() {
            do {
                if (!pimpl->active.load()) return;
                std::this_thread::sleep_for(pimpl->interval);
                if (!pimpl->active.load()) return;
                pimpl->function();
            } while (pimpl->type == repeat);
            pimpl->active.store(false);
        });
        t.detach();
    }

    void cancel() {
        pimpl->active.store(false);
    }

    bool isActive() {
        return pimpl->active.load();
    }

private:
    struct Impl {
        Callback function;
        Interval interval;
        Type type;
        std::atomic<bool> active{false};
    };

    std::shared_ptr<Impl> pimpl;
};
