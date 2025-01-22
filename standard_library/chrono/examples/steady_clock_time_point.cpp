// time_point constructors
#include <iostream>
#include <chrono>
#include <ctime>

int main ()
{
    using namespace std::chrono;

    steady_clock::time_point tp_epoch;	// epoch value
    if (tp_epoch == steady_clock::time_point{}) {
        std::cout << "tp_epoch is equal steady_clock::time_point{}\n";
    } else {
        std::cout << "tp_epoch is not equal steady_clock::time_point{}\n";
    }

    time_point <steady_clock,duration<int>> tp_seconds (duration<int>(1));

    steady_clock::time_point tp (tp_seconds);

    std::cout << "1 second since steady_clock epoch = ";
    std::cout << tp.time_since_epoch().count();
    std::cout << " steady_clock periods." << std::endl;


    return 0;
}
