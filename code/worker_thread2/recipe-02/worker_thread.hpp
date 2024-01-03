#pragma once

#include <thread>
#include <atomic>
#include <functional>

#include "threadsafe_queue.hpp"

class worker_thread {
private:
    std::atomic_bool done;
    threadsafe_queue<std::function<void()>> work_queue;
    std::thread work_thread;

    void work_loop()
    {
        while(!done)
        {
            std::function<void()> task;
            work_queue.wait_and_pop(task);
            task();
        }
    }

    void stop()
    {
        done = true;
        std::function<void()> stop_task =
            [this]() {
                this->done = true;
            };
        work_queue.push(stop_task);
    }

public:
    worker_thread(): done(false)
    {
        work_thread = std::thread(&worker_thread::work_loop, this);
    }

    ~worker_thread()
    {
        stop();
        work_thread.join();
    }

    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(std::function<void()>(f));
    }
};
