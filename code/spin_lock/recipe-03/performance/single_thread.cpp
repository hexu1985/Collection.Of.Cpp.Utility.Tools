#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <list>
#include <cstdint>
#include "spin_lock.hpp"

static constexpr size_t loop_max = 10000000;
static std::list<int> g_list;

template <typename LockType>
void test_lock(const char *lock_name)
{
    auto start = std::chrono::steady_clock::now();
    LockType locker;
    for (size_t i = 0; i < loop_max; i++)
    {
        std::lock_guard<LockType> lock_guard(locker);
        g_list.emplace_back(i);
        g_list.pop_back();
    }
    std::int64_t ms = std::chrono::duration_cast<std::chrono::microseconds>
                        (std::chrono::steady_clock::now() - start).count();
    std::cout << "lock_type   " << lock_name << "\n";
    std::cout << "elapsed     " << ms / 1000000.0 << "\n\n";
}

int main()
{
    test_lock<std::mutex>("std::mutex");
    test_lock<SpinLock>("SpinLock");
    return 0;
}
