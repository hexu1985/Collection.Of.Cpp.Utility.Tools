#pragma once

#include <pthread.h>
#include <assert.h>

class InterprocessCondition {
public:
    InterprocessCondition();
    ~InterprocessCondition();

    void notify_one();
    void notify_all();

    template <typename L>
    void wait(L& lock) {
        assert(lock);
        wait_impl(lock.mutex()->native_handle());
    }

    template <typename L, typename Pr>
    void wait(L& lock, Pr pred) {
        while (!pred()) {
            wait(lock);
        }
    }

private:
    void wait_impl(pthread_mutex_t* mutex);

private:
    pthread_cond_t cnd_;
};
