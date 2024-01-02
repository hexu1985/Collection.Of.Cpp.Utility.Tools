#pragma once

#include <thread>
#include <atomic>

#include "threadsafe_queue.hpp"

class worker_thread {
private:
    std::atomic_bool done;
    threadsafe_queue<std::function<void()>> work_queue;
    std::thread work_thread;

    struct interrupt_thread_exception {};

    static void interrupt_thread()
    {
        throw interrupt_thread_exception{};
    }

    void work_loop()
    {
        while (true)
        {
            std::function<void()> task;
            work_queue.wait_and_pop(task);
            try 
            {
                task();
            }
            catch (interrupt_thread_exception)
            {
                return;
            }
        }
    }

public:
    worker_thread()
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

    void stop()
    {
        work_queue.push(std::function<void()>(
                    &worker_thread::interrupt_thread));
    }
};
