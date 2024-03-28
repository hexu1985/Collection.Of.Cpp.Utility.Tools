#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>

using namespace std;
using namespace std::chrono;

int plus_one(int x) {
    this_thread::sleep_for(milliseconds(500));
    return x + 1;
}

int mul_two(int x) {
    this_thread::sleep_for(milliseconds(500));
    return x * 2;
}

std::string print(int x) {
    this_thread::sleep_for(milliseconds(500));
    return std::to_string(x);
}


int main() {
    cout << fixed << setprecision(1);
    for (int i = 0; i < 5; i++) {
        auto start_time = system_clock::now();
        auto output = print(mul_two(plus_one(i)));
        auto end_time = system_clock::now();
        cout << output << ": " << duration<double>(end_time-start_time).count() << "s" << endl;
    }
}
