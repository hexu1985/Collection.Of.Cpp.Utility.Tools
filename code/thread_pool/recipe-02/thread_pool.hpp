#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include "threadsafe_queue.hpp"

class join_threads
{
    std::vector<std::thread>& threads;

public:
    explicit join_threads(std::vector<std::thread>& threads_):
        threads(threads_)
    {}

    ~join_threads()
    {
        for(unsigned long i=0;i<threads.size();++i)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }

    size_t thread_count() { return threads.size(); }
};

class thread_pool
{
    std::atomic_bool done;
    threadsafe_queue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread()
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
        for (unsigned i=0; i<joiner.thread_count(); i++) {
            work_queue.push(stop_task);
        }
    }

public:
    thread_pool(unsigned const thread_count=std::thread::hardware_concurrency()):
        done(false),joiner(threads)
    {
        try
        {
            for(unsigned i=0;i<thread_count;++i)
            {
                threads.push_back(
                    std::thread(&thread_pool::worker_thread,this));
            }
        }
        catch(...)
        {
            stop();
            throw;
        }
    }

    ~thread_pool()
    {
        stop();
    }

    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(std::function<void()>(f));
    }
};
