#include "interprocess_once.hpp"

#include <thread>
#include <mutex>
#include <chrono>

using namespace std::literals;

namespace {

class SpinLockRef {
public:
    SpinLockRef(std::atomic_bool& flag): flag_(flag) {
    }

    void lock() {
        while (flag_.exchange(true, std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        flag_.store(false, std::memory_order_release);
    }

    bool try_lock() {
        return !flag_.exchange(true, std::memory_order_acquire);
    }

private:
    std::atomic_bool& flag_;
};

enum class ExecuteState {
    Not_yet_executed = 0,
    Executing = 1,
    Executed = 2,
};

}   // namespace

void interprocess_call_once(InterprocessOnceFlag& flag, std::function<void()> fn) {
    SpinLockRef lock(flag.lock);
    while (true) {
        std::unique_lock<SpinLockRef> scoped_lock(lock);

        int& execute_state = flag.execute_state;
        if (execute_state == (int) ExecuteState::Executed) {
            break;
        }

        if (execute_state == (int) ExecuteState::Executing) {
            scoped_lock.unlock();
            std::this_thread::sleep_for(10us);
            continue;
        }

        if (execute_state == (int) ExecuteState::Not_yet_executed) {
            execute_state = (int) ExecuteState::Executing;
        }

        try {
            fn();
            execute_state = (int) ExecuteState::Executed;
            break;
        } catch (...) {
            execute_state = (int) ExecuteState::Not_yet_executed;
            throw;  // rethrow
        }
    }
}

