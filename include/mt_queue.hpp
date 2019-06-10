#ifndef MINI_UTILSMT_QUEUE_INC
#define MINI_UTILSMT_QUEUE_INC

#include <mutex>
#include <condition_variable>
#include <queue>

namespace MiniUtils {

template <typename T>
class MTQueue {
private:
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<T> queue_;

public:
    MTQueue() = default;

    MTQueue(const MTQueue& other) = delete;

    void push(const T &new_value)
    {
        std::lock_guard<std::mutex> lk(mtx_);
        queue_.push(new_value);
        cv_.notify_one();
    }

    void push(T &&new_value)
    {
        std::lock_guard<std::mutex> lk(mtx_);
        queue_.push(std::forward<T>(new_value));
        cv_.notify_one();
    }

    void pop(T &value)
    {
        std::unique_lock<std::mutex> lk(mtx_);
        cv_.wait(lk, [&]{return !queue_.empty();});
        /*
           while (queue_.empty()) {
           cv_.wait(lk);
           }
           */
        value = std::move(queue_.front());
        queue_.pop();
    }

    T pop()
    {
        T val;
        pop(val);
        return std::move(val);
    }

    bool tryPop(T &value)
    {
        std::lock_guard<std::mutex> lk(mtx_);
        if (queue_.empty()) 
            return false;
        value = std::move(*queue_.front());
        queue_.pop();
    }

    bool isEmpty() const
    {
        std::lock_guard<std::mutex> lk(mtx_);
        return queue_.empty();
    }
};

}   // namespace MiniUtils

#endif
