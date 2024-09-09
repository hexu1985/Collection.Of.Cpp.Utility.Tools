#include "interprocess_semaphore.hpp"
#include <mutex>

InterprocessSemaphore::InterprocessSemaphore(unsigned int value): count_(value) {
}

InterprocessSemaphore::~InterprocessSemaphore() {
}

void InterprocessSemaphore::post() {
    std::unique_lock<InterprocessMutex> lck{mutex_};
    cond_.notify_one();
    count_++;
}

void InterprocessSemaphore::wait() {
    std::unique_lock<InterprocessMutex> lck{mutex_};
    while (count_ == 0) {
        cond_.wait(lck);
    }
    count_--;
}

bool InterprocessSemaphore::try_wait() {
    std::unique_lock<InterprocessMutex> lck{mutex_};
    if (count_ > 0) {
        count_--;
        return true;
    } else {
        return false;
    }
}

int InterprocessSemaphore::get_value() {
    int value;
    {
        std::unique_lock<InterprocessMutex> lck{mutex_};
        value = count_;
    }
    return value;
}
