#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class limitedsize_queue {
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable not_empty_cond;
    std::condition_variable not_full_cond;
    size_t maxsize;

    enum {
        default_maxsize = 50
    };

public:
    limitedsize_queue(size_t maxsize_=default_maxsize): maxsize(maxsize_)
    {}

    void push(T new_value)
    {
        std::unique_lock<std::mutex> lk(mut);
        not_full_cond.wait(lk,[this]{return !is_full();});

        data_queue.push(std::move(new_value));
        not_empty_cond.notify_one();
    }

    void pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        not_empty_cond.wait(lk,[this]{return !is_empty();});

        value=std::move(data_queue.front());
        data_queue.pop();
        not_full_cond.notify_one();
    }

    bool try_push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (is_full()) {
            return false;
        }

        data_queue.push(std::move(new_value));
        not_empty_cond.notify_one();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (is_empty()) {
            return false;
        }

        value=std::move(data_queue.front());
        data_queue.pop();
        not_full_cond.notify_one();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return is_empty();
    }

    bool full() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return is_full();
    }

    size_t qsize() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.size();
    }

    size_t max_size() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return maxsize;
    }

private:
    bool is_empty() const 
    {
        return data_queue.empty();
    }

    bool is_full() const
    {
        return data_queue.size() >= maxsize;
    }
};

