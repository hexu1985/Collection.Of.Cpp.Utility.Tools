/** \example stopwatch/sample_timer_base_chrono1.cpp
 * This is an example of how to use the TimerBaseSystem class.
 */
#include <iostream>
#include <thread>
#include "Stopwatch.hpp"

using namespace mini_util;

int main ()
{
  TimerBaseChrono timer;
  timer.Start();

  std::this_thread::sleep_for(std::chrono::seconds(3));

  auto time_span = timer.GetMs();

  std::cout << "It took me " << (double) time_span/1000 << " s." << '\n';
  std::cout << "It took me " << time_span << " ms." << '\n';
  std::cout << std::endl;

  return 0;
}

