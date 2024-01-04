#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <future>
#include <type_traits>
#include "threadsafe_queue.hpp"

class function_wrapper
{
    struct impl_base {
        virtual void call()=0;
        virtual ~impl_base() {}
    };
    std::unique_ptr<impl_base> impl;
    template<typename F>
    struct impl_type: impl_base
    {
        F f;
        impl_type(F&& f_): f(std::move(f_)) {}
        void call() { f(); }
    };

public:
    function_wrapper() {}

    template<typename F>
    function_wrapper(F&& f):
        impl(new impl_type<F>(std::move(f)))
    {}

    void operator()() { impl->call(); }

    function_wrapper(function_wrapper&& other):
        impl(std::move(other.impl))
    {}

    function_wrapper& operator=(function_wrapper&& other)
    {
        impl=std::move(other.impl);
        return *this;
    }

    function_wrapper(const function_wrapper&)=delete;
    function_wrapper(function_wrapper&)=delete;
    function_wrapper& operator=(const function_wrapper&)=delete;
};

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
};

class thread_pool
{
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread()
    {
        while(!done)
        {
            function_wrapper task;
            work_queue.wait_and_pop(task);
            task();
        }
    }

    void stop()
    {
        done = true;
        auto thread_count = threads.size();
        for (unsigned i=0; i<thread_count; i++) {
            auto stop_task =
                [this]() {
                    this->done = true;
                };
            work_queue.push(std::move(stop_task));
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
    std::future<typename std::invoke_result<FunctionType>::type>
    submit(FunctionType f)
    {
        typedef typename std::invoke_result<FunctionType>::type result_type;
        
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push(std::move(task));
        return res;
    }

    template <typename F, typename... Args>
    std::future<typename std::invoke_result<F, Args...>::type> 
    submit(F&& f, Args&&... args)
    {
        typedef typename std::invoke_result<F, Args...>::type result_type;
        
        std::packaged_task<result_type()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<result_type> res(task.get_future());
        work_queue.push(std::move(task));
        return res;
    }
};
