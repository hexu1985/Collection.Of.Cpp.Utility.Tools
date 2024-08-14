#include "interprocess_condition.hpp"
#include <system_error>

namespace {

struct InterprocessConditionAttr {
    InterprocessConditionAttr() {
        int n;
        n = pthread_condattr_init(&attr);
        if (n != 0) {
            throw std::system_error(n, std::system_category(), "pthread_condattr_init error");
        }

        n = pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        if (n != 0) {
            throw std::system_error(n, std::system_category(), "pthread_condattr_setpshared error");
        }
    }

    ~InterprocessConditionAttr()  {  pthread_condattr_destroy(&attr);  }

    pthread_condattr_t* native_handle() { return &attr; }

    pthread_condattr_t attr;
};

}   // namespace

InterprocessCondition::InterprocessCondition() {
    InterprocessConditionAttr attr;
    int n = pthread_cond_init(&cnd_, attr.native_handle());
    if (n != 0) {
        throw std::system_error(n, std::system_category(), "pthread_cond_init error");
    }
}

InterprocessCondition::~InterprocessCondition() {
    pthread_cond_destroy(&cnd_);
}

void InterprocessCondition::notify_one() {
    int n = pthread_cond_signal(&cnd_);
    if (n != 0) {
        throw std::system_error(n, std::system_category(), "pthread_cond_signal error");
    }
}

void InterprocessCondition::notify_all() {
    int n = pthread_cond_broadcast(&cnd_);
    if (n != 0) {
        throw std::system_error(n, std::system_category(), "pthread_cond_broadcast error");
    }
}

void InterprocessCondition::wait_impl(pthread_mutex_t* mutex) {
    int n = pthread_cond_wait(&cnd_, mutex);
    if (n != 0) {
        throw std::system_error(n, std::system_category(), "pthread_cond_wait error");
    }
}

