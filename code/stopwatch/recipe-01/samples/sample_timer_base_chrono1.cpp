#include <iostream>
#include <thread>
#include "stopwatch.hpp"

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

