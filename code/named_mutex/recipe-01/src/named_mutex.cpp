#include "named_mutex.hpp"

#include <errno.h>
#include <system_error>
#include <functional>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

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

}   // namespace

NamedMutex::NamedMutex(const char* name): handle_(name) {
    pthread_mutex_t* mtx = &handle_.get().mutex;

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

NamedMutex::~NamedMutex() {
}

void NamedMutex::lock() {
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

void NamedMutex::unlock() {
    pthread_mutex_t* mtx = &handle_.get().mutex;

    int n = pthread_mutex_unlock(mtx);
    if (n != 0) {
        throw std::system_error(n, std::system_category(), "pthread_mutex_unlock error");
    }
}

bool NamedMutex::try_lock() {
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

bool NamedMutex::remove(const char* name) {
    return SharedMemoryObject::remove(name); 
}
