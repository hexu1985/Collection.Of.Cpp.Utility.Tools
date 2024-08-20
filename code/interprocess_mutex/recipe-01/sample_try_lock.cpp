// mutex::try_lock example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include "interprocess_mutex.hpp"

volatile int counter (0); // non-atomic counter
InterprocessMutex mtx;           // mutex for critical section

void attempt_10k_increases () {
    for (int i=0; i<10000; ++i) {
        if (mtx.try_lock()) {   // only increase if currently not locked:
            ++counter;
            mtx.unlock();
        }
    }
}

int main ()
{
    std::thread threads[10];
    // spawn 10 threads:
    for (int i=0; i<10; ++i)
        threads[i] = std::thread(attempt_10k_increases);

    for (auto& th : threads) th.join();
    std::cout << counter << " successful increases of the counter.\n";

    return 0;
}
