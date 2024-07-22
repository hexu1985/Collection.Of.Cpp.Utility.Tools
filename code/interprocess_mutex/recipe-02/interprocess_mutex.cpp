#include "interprocess_mutex.hpp"

#include <errno.h>
#include <system_error>
#include <functional>
#include <thread>
#include <chrono>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;
using namespace std::literals;

namespace {

template <typename Handle>
class ScopedHandleGuard {
public:
    using DestroyFunc = std::function<void (Handle*)>;
    ScopedHandleGuard(Handle* handle, DestroyFunc destory_func):
        handle_(handle), destory_func_(destory_func) {}

    ~ScopedHandleGuard() {
        destory_func_(handle_);
    }

private:
    Handle* handle_;
    DestroyFunc destory_func_;
};

enum class InitState {
    uninitialized = 0,
    initializing = 1,
    initialized = 2,
};

void Pthread_mutex_init(pthread_mutex_t* mtx) {
    pthread_mutexattr_t	mattr;
    int n;
    if ((n = pthread_mutexattr_init(&mattr)) != 0) {
        throw std::system_error(n, std::system_category(), "pthread_mutexattr_init error");
    }

    ScopedHandleGuard<pthread_mutexattr_t> mattr_guard(&mattr, [](pthread_mutexattr_t* attr) { pthread_mutexattr_destroy(attr); });
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);

    if ((n = pthread_mutex_init(mtx, &mattr)) != 0) {
        throw std::system_error(n, std::system_category(), "pthread_mutex_init error");
    }
}

}   // namespace

InterprocessMutex::InterprocessMutex(const char* name): handle_(name) {
    std::atomic_flag& flag = handle_.get().flag;

    while (true) {
        while (flag.test_and_set(std::memory_order_acquire)) {  // lock
            std::this_thread::yield();
        }

        // auto unlock
        ScopedHandleGuard<std::atomic_flag> flag_guard(&flag, [](std::atomic_flag* pflag) { pflag->clear(std::memory_order_release); });

        int& state = handle_.get().state;
        if (state == (int) InitState::initialized) {
            break;
        }

        if (state == (int) InitState::initializing) {
            std::this_thread::sleep_for(10ms);
            continue;
        }

        if (state == (int) InitState::uninitialized) {
            state = (int) InitState::initializing;
        }

        try {
            pthread_mutex_t* mtx = &handle_.get().mutex;
            Pthread_mutex_init(mtx);
            state = (int) InitState::initialized;
            break;
        } catch (...) {
            state = (int) InitState::uninitialized;
            throw;  // rethrow;
        }
    }
}

InterprocessMutex::~InterprocessMutex() {
}

void InterprocessMutex::lock() {
    pthread_mutex_t* mtx = &handle_.get().mutex;

    while (true) {
        int n = pthread_mutex_lock(mtx);
        if (n == 0) {
            break;
        } else if (n == EINTR) {
            continue;
        } else {
            throw std::system_error(n, std::system_category(), "pthread_mutex_lock error");
        }
    }
}

void InterprocessMutex::unlock() {
    pthread_mutex_t* mtx = &handle_.get().mutex;

    int n = pthread_mutex_unlock(mtx);
    if (n != 0) {
        throw std::system_error(n, std::system_category(), "pthread_mutex_unlock error");
    }
}

bool InterprocessMutex::try_lock() {
    pthread_mutex_t* mtx = &handle_.get().mutex;

    int n = pthread_mutex_trylock(mtx);
    if (n == 0) {
        return true;
    } else if (n == EBUSY) {
        return false;
    } else {
        throw std::system_error(n, std::system_category(), "pthread_mutex_trylock error");
    }
    return false;
}

bool InterprocessMutex::remove(const char* name) {
    return SharedMemory::remove(name); 
}
