#include <iostream>
#include <list>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>
#include "spin_lock.hpp"

static std::list<int> g_list;
static constexpr size_t loop_max = 10000000;

template <typename LockType>
void test_thread(size_t id, LockType &lock)
{
    for (size_t i = 0; i < loop_max; i++)
    {
        std::lock_guard<LockType> lock_guard(lock);
        g_list.push_back(id);
        g_list.pop_back();
    }
}

template <typename LockType>
void begin_test(size_t thread_number, const char *lock_name)
{
    std::vector<std::thread> threads;
    LockType lock;

    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < thread_number; i++)
    {
        threads.emplace_back([i, &lock](){
            test_thread(i, lock);
        });
    }
    for (auto &t : threads)
    {
        t.join();
    }
 
 
    std::int64_t ms = std::chrono::duration_cast<std::chrono::microseconds>
                        (std::chrono::steady_clock::now() - start).count();
    std::cout << "lock_type   " << lock_name << "\n";
    std::cout << "elapsed     " << ms / 1000000.0 << "\n\n";
}

int main()
{
    size_t nproc = std::thread::hardware_concurrency();
    std::cout << "nproc: " << nproc << "\n\n";
    for (size_t n = 1; n <= 2*nproc; n++)
    {
        std::cout << "=============== " << n << " ===============\n";
        begin_test<std::mutex>(n, "std::mutex");
        begin_test<SpinLock>(n, "SpinLock");
    }
    return 0;
}
