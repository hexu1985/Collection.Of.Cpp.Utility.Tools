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

std::string print(int x) {
    this_thread::sleep_for(milliseconds(500));
    return std::to_string(x);
}

int main() {
    SimplePipeline<int, std::string> pipeline;
    pipeline.add_data_source(std::function<bool(int&)>{data_provider{}})
            .add_data_filter(std::function<int(int)>{plus_one})
            .add_data_filter(std::function<int(int)>{mul_two})
            .add_data_filter(std::function<std::string(int)>{print});
    pipeline.start();
    cout << fixed << setprecision(1);
    std::string output;
    while (true) {
        auto start_time = system_clock::now();
        pipeline.get(output);
        auto end_time = system_clock::now();
        cout << output << ": " << duration<double>(end_time-start_time).count() << "s" << endl;
    }
}
