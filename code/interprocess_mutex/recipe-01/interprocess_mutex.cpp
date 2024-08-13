#include "interprocess_mutex.hpp"
#include <system_error>

namespace {

struct InterprocessMutexAttr {
    InterprocessMutexAttr() {
        int n;
        n = pthread_mutexattr_init(&attr);
        if (n != 0) {
            throw std::system_error(n, std::system_category(), "pthread_mutexattr_init error");
        }

        n = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        if (n != 0) {
            throw std::system_error(n, std::system_category(), "pthread_mutexattr_setpshared error");
        }
    }

    ~InterprocessMutexAttr()  {  pthread_mutexattr_destroy(&attr);  }

    pthread_mutexattr_t* native_handle() { return &attr; }

    pthread_mutexattr_t attr;
};

}   // namespace

InterprocessMutex::InterprocessMutex() {
    InterprocessMutexAttr attr;
    int n = pthread_mutex_init(&mtx_, attr.native_handle());
    if (n != 0) {
        throw std::system_error(n, std::system_category(), "pthread_mutex_init error");
    }
}

InterprocessMutex::~InterprocessMutex() {
    pthread_mutex_destroy(&mtx_);
}

void InterprocessMutex::lock() {
    while (true) {
        int n = pthread_mutex_lock(&mtx_);
        if (n == 0) {
            break;
        } else if (n == EINTR) {     
            continue;
        } else {
            throw std::system_error(n, std::system_category(), "pthread_mutex_lock error");
        }
    }
}

bool InterprocessMutex::try_lock() {
    int n = pthread_mutex_trylock(&mtx_);
    if (n == 0) {
        return true;
    } else if (n == EBUSY) {
        return false;
    } else {
        throw std::system_error(n, std::system_category(), "pthread_mutex_trylock error");
    }
    return false;
}

void InterprocessMutex::unlock() {
    int n = pthread_mutex_unlock(&mtx_);
    if (n != 0) {
        throw std::system_error(n, std::system_category(), "pthread_mutex_unlock error");
    }
}

