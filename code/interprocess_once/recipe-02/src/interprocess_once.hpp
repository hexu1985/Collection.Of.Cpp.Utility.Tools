#pragma once

#include <atomic>
#include <functional>

static_assert(ATOMIC_BOOL_LOCK_FREE == 2, "atomic_bool need lock free");

struct InterprocessOnceFlag {
    std::atomic_bool lock;
    int execute_state = 0;
};

void interprocess_call_once(InterprocessOnceFlag& flag, std::function<void()> fn);

