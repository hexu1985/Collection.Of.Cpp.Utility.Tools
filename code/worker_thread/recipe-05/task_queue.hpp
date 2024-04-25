#pragma once

#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>

using Task = std::function<void()>;

class TaskQueue {
public:
    using InternalQueueType = std::deque<Task>;

private:
    InternalQueueType queue;
    std::mutex queue_mtx;
    std::condition_variable queue_cv;

public:
    template <typename Fn, typename... Args>
    void PushTask(Fn&& fn, Args&&... args)
    {
        PushTask(Task(std::bind<void>(std::forward<Fn>(fn), std::forward<Args>(args)...)));
    }

    void PushTask(Task&& task) 
    {
        std::lock_guard<std::mutex> lck(queue_mtx);
        queue.push_back(std::move(task));
        queue_cv.notify_one();
    }

    Task PopTask() 
    {
        std::unique_lock<std::mutex> lck(queue_mtx);
        while (queue.empty()) {
            queue_cv.wait(lck);
        }
        auto task = std::move(queue.front());
        queue.pop_front();
        return task;
    }

    void PopTask(InternalQueueType& task_list)
    {
        std::unique_lock<std::mutex> lck(queue_mtx);
        while (queue.empty()) {
            queue_cv.wait(lck);
        }
        queue.swap(task_list);
        queue.clear();
    }
};

