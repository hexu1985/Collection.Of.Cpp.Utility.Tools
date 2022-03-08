#include <iostream>
#include <thread>

#include "timer.h"

using namespace std;

int main()
{
    timer t;			// time recursive version

    // evaluate fib() using iteration and recursion
    cout << "sleep 20 ms ";
    // start/stop timer object for recursive process
    t.start();
    this_thread::sleep_for(std::chrono::milliseconds(20));
    t.stop();

    cout << "   Time escaped " << t.time() << " sec" <<endl;

    return 0;
}
