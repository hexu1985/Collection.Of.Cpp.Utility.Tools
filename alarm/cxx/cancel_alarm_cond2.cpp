// c++ program to explain the
// use of cancel() method in Timer class

#include "cond_timer.hpp"
#include <functional>
#include <iostream>
#include <thread>
#include <chrono>

void helper_function(int i) {
    std::cout << "Value printed=" << i << std::endl;
}

int main()
{
    auto timer1 = Timer(3, std::bind(helper_function, 19));
    std::cout << "Starting the timer object\n";
    std::cout << std::endl;

    // Starting the function after 3 seconds
    timer1.Start();

    // Sleeping this thread for 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // This will not cancel the thread as 3 seconds have passed
    timer1.Cancel();
    std::cout << "This time thread is not cancelled as 3 seconds have passed when cancel() method is called\n";

    quick_exit(0);
}

