#include "Thread.hpp"

Thread::Thread() {
}

Thread::~Thread() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

void Thread::start() {
    thread_ = std::thread(&Thread::run, this);
}

void Thread::join() {
    thread_.join();
}
