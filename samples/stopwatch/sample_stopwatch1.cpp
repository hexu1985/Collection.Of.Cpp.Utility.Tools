/** \example sample_stopwatch1.cpp
 * This is an example of how to use the Stopwatch class.
 */
#include <iostream>
#include <chrono>
#include <thread>
#include "Stopwatch.hpp"

using namespace MiniUtils;

void foo()
{
    Stopwatch sw("foo");
    // process
    std::cout << "in foo()" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void bar()
{
    Stopwatch sw("bar");
    // process
    std::cout << "in bar()" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main()
{
    Stopwatch sw("main");
    foo();
    sw.show("foo complete at");

    bar();
    sw.show("bar complete at");

    return 0;
}
