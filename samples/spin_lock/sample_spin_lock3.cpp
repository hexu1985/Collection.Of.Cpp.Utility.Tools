/** \example spin_lock/sample_spin_lock3.cpp
 * This is an example of how to use the SpinLock class's tryLock method.
 */
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include "SpinLock.hpp"  // SpinLock

using namespace mini_utils;

volatile int counter (0); // non-atomic counter
SpinLock mtx;           // locks access to counter

void attempt_10k_increases () {
  for (int i=0; i<10000; ++i) {
    if (mtx.tryLock()) {   // only increase if currently not locked:
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

/*
Possible output (any count between 1 and 100000 possible):

80957 successful increases of the counter.
*/
