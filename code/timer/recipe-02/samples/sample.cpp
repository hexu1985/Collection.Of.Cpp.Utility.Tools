#include <iostream>
#include "timer.hpp"

using namespace std;

int main() {
    Timer t1;

    t1.setInterval([&]() {
        cout << "Hey.. After each 1s..." << endl;
    }, chrono::milliseconds(1000)); 

    Timer t2;
    t2.setTimeout([&]() {
        cout << "Hey.. After 5.2s. But I will stop the timer!" << endl;
        t1.stop();
        cout << "I stoped the timer!" << endl;
    }, chrono::milliseconds(5200)); 

    

    cout << "I am Timer" <<endl;


    while(true); // Keep mail thread active
}
