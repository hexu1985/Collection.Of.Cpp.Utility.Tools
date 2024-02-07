// c++ program to explain the
// use of cancel() method in Timer class

#include "timer.hpp"
#include <functional>
#include <iostream>

void helper_function(int i) {
    std::cout << "Value printed=" << i << std::endl;
}

int main()
{
    Timer timer1;
    std::cout << "Starting the timer object\n";
    std::cout << std::endl;

    // Starting the function after 3 seconds
    timer1.setTimeout(std::bind(helper_function, 9), 3000);

    std::cout << "This gets printed before the helper_function as helper_function starts after 3 seconds\n";
    std::cout << std::endl;

    // This cancels the thread when 3 seconds 
    // have not passed
    timer1.stop();
    std::cout << "Thread1 cancelled, helper_function is not executed\n";

    std::cin.get();
}
