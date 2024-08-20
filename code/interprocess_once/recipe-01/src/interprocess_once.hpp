#pragma once

#include <atomic>
#include <functional>

struct InterprocessOnceFlag {
    std::atomic_flag lock;
    int execute_state = 0;
};

void interprocess_call_once(InterprocessOnceFlag& flag, std::function<void()> fn);

