#pragma once

#include <thread>
#include <atomic>

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
            if(work_queue.try_pop(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

public:
    worker_thread(): done(false)
    {
        work_thread = std::thread(&worker_thread::work_loop, this);
    }

    ~worker_thread()
    {
        done=true;
        work_thread.join();
    }

    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(std::function<void()>(f));
    }
};
