#pragma once

#include <thread>
#include <chrono>
#include <atomic>
#include <functional>
#include <memory>

class Timer {
    std::atomic<bool> active{true};

public:
    using Callback = std::function<void ()>;
    using Interval = std::chrono::milliseconds;

    enum Type {
        once = 1,
        repeat = 2,
    };

    Timer(Callback function, Interval interval, Type type=once) {
        pimpl = std::make_shared<Impl>();
        pimpl->function = function;
        pimpl->interval = interval;
        pimpl->type = type;

        std::weak_ptr<Impl> timer{pimpl};
        std::thread t([timer]() {
            auto pimpl = timer.lock();
            if (!pimpl) return;
            do {
                if (pimpl->done.load()) return;
                std::this_thread::sleep_for(pimpl->interval);
                if (pimpl->done.load()) return;
                pimpl->function();
            } while (pimpl->type == repeat);
            pimpl->done.store(true);
        });
        t.detach();
    }

    void stop() {
        pimpl->done.store(true);
    }

    bool isRunning() {
        return !pimpl->done.load();
    }

private:
    struct Impl {
        Callback function;
        Interval interval;
        Type type;
        std::atomic<bool> done{false};
    };

    std::shared_ptr<Impl> pimpl;
};


