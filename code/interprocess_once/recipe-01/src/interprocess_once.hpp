#pragma once

#include <atomic>
#include <functional>

struct InterprocessOnceFlag {
    std::atomic_flag lock;
    int execute_state = 0;
};

void CallOnce(InterprocessOnceFlag& flag, std::function<void()> fn);
