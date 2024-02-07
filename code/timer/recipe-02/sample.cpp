#include <iostream>
#include "timer.hpp"

using namespace std;

int main() {
    Timer t;

    t.setInterval([&]() {
        cout << "Hey.. After each 1s..." << endl;
    }, 1000); 

    t.setTimeout([&]() {
        cout << "Hey.. After 5.2s. But I will stop the timer!" << endl;
        t.stop();
        cout << "I stoped the timer!" << endl;
    }, 5200); 

    

    cout << "I am Timer" <<endl;

    cin.get();   // Keep mail thread active
}
