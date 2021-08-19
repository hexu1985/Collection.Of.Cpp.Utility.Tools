#include <iostream>
#include "timer.hpp"

using namespace mini_util;
using namespace std;

int main() {
    Timer t;

    t.SetInterval([&]() {
        cout << "Hey.. After each 1s..." << endl;
    }, chrono::milliseconds(1000)); 

    t.SetTimeout([&]() {
        cout << "Hey.. After 5.2s. But I will stop the timer!" << endl;
        t.Stop();
    }, chrono::milliseconds(5200)); 

    

    cout << "I am Timer" <<endl;


    while(true); // Keep mail thread active
}
