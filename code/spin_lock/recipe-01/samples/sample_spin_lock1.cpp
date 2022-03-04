#include <iostream>                 // std::cout
#include <thread>                   // std::thread
#include "spin_lock.hpp"            // SpinLock

SpinLock mtx;                       // SpinLock for critical section

void print_thread_id (int id) {
  // critical section (exclusive access to std::cout signaled by locking mtx):
  mtx.lock();
  std::cout << "thread #" << id << '\n';
  mtx.unlock();
}

int main ()
{
  std::thread threads[10];
  // spawn 10 threads:
  for (int i=0; i<10; ++i)
    threads[i] = std::thread(print_thread_id,i+1);

  for (auto& th : threads) th.join();

  return 0;
}

/*
Possible output (order of lines may vary, but they are never intermingled):
thread #1
thread #2
thread #3
thread #4
thread #5
thread #6
thread #7
thread #8
thread #9
thread #10
*/
