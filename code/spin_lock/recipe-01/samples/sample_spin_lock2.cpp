/** \example spin_lock/sample_spin_lock2.cpp
 * This is an example of how to use the SpinLock class.
 */
#include <iostream>                     // std::cout
#include <thread>                       // std::thread
#include "spin_lock.hpp"                // SpinLock

using namespace mini_util;

SpinLock mtx;                           // SpinLock for critical section

void print_block (int n, char c) {
  // critical section (exclusive access to std::cout signaled by locking mtx):
  mtx.lock();
  for (int i=0; i<n; ++i) { std::cout << c; }
  std::cout << '\n';
  mtx.unlock();
}

int main ()
{
  std::thread th1 (print_block,50,'*');
  std::thread th2 (print_block,50,'$');

  th1.join();
  th2.join();

  return 0;
}

/*
Possible output (order of lines may vary, but characters are never mixed):

**************************************************
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/
