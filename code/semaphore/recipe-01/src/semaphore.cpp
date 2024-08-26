#include "semaphore.hpp"

Semaphore::Semaphore(unsigned int value): count_(value) {
}

Semaphore::~Semaphore() {
}

void Semaphore::post() {
    std::unique_lock<std::mutex> lck{mutex_};
    cond_.notify_one();
    count_++;
}

void Semaphore::wait() {
    std::unique_lock<std::mutex> lck{mutex_};
    while (count_ == 0) {
        cond_.wait(lck);
    }
    count_--;
}

bool Semaphore::try_wait() {
    std::unique_lock<std::mutex> lck{mutex_};
    if (count_ > 0) {
        count_--;
        return true;
    } else {
        return false;
    }
}

