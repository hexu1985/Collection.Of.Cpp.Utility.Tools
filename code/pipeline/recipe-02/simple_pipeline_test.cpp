#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include "simple_pipeline.hpp"

using namespace std;
using namespace std::chrono;

class data_provider {
public:
    data_provider(): i(0) {}

    bool operator() (int& value)
    {
        if (i >= 5) {
            return false;
        }
        value = i;
        i += 1;
        return true;
    }

private:
    int i;
};

int plus_one(int x) {
    this_thread::sleep_for(milliseconds(500));
    return x + 1;
}

int mul_two(int x) {
    this_thread::sleep_for(milliseconds(500));
    return x * 2;
}

int main() {
    std::vector<std::function<int(int)>> funcs;
    funcs.push_back(plus_one);
    funcs.push_back(mul_two);

    SimplePipeline<int> pipeline(data_provider{}, funcs);
    pipeline.start();
    cout << fixed << setprecision(1);
    int output;
    while (true) {
        auto start_time = system_clock::now();
        pipeline.get(output);
        auto end_time = system_clock::now();
        cout << output << ": " << duration<double>(end_time-start_time).count() << "s" << endl;
    }
}
