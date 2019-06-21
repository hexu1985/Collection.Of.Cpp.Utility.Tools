/** \example sample_timer_base_chrono3.cpp
 * This is an example of how to use the TimerBaseHighResolution class.
 */
#include <iostream>
#include <thread>
#include "TimerBaseChrono.hpp"

using namespace MiniUtils;

int main ()
{
  TimerBaseHighResolution timer;
  timer.start();

  std::this_thread::sleep_for(std::chrono::seconds(3));

  auto time_span = timer.getMs();

  std::cout << "It took me " << (double) time_span/1000 << " s." << '\n';
  std::cout << "It took me " << time_span << " ms." << '\n';
  std::cout << std::endl;

  return 0;
}

