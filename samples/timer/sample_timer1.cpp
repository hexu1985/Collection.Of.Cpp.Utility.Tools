#include <iostream>
#include "Timer.hpp"

using namespace std;
using namespace mini_utils;

int main() 
{
    Timer t;

    t.setInterval([&]() {
        cout << "Hey.. After each 1s..." << endl;
    }, chrono::seconds(1)); 

    t.setTimeoutFor([&]() {
        cout << "Hey.. After 5.2s. But I will stop the timer!" << endl;
        t.stop();
    }, chrono::milliseconds(5200)); 

	t.setTimeoutUntil([&]() {
        cout << "Hey.. After 2s." << endl;
	}, chrono::system_clock::now() + chrono::seconds(2));

    cout << "I am Timer" << endl;

	cout << "any key to stop" << endl;
	cin.get();

	return 0;
}
