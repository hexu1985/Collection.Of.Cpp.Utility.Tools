#include "Thread.hpp"

Thread::Thread() {
}

Thread::Thread(Thread&& x): thread(std::move(x.thread)) {
}

Thread::~Thread() {
    if (thread.joinable()) {
        thread.join();
    }
}

void Thread::start() {
    thread = std::thread(&Thread::run, this);
}

void Thread::join() {
    thread.join();
}
