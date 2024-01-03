#pragma once

#include <thread>
#include <atomic>
#include <future>

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


class worker_thread {
private:
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> work_queue;
    std::thread work_thread;

    void work_loop()
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
        auto stop_task =
            [this]() {
                this->done = true;
            };
        work_queue.push(std::move(stop_task));
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
    std::future<typename std::result_of<FunctionType()>::type>
    submit(FunctionType f)
    {
        typedef typename std::result_of<FunctionType()>::type result_type;
        
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push(std::move(task));
        return res;
    }
};
