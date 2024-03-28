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
    const size_t capacity_per_pipe = 1;
    Pipe<int> in_pipe = make_pipe<int>(capacity_per_pipe);
    auto data_source = std::make_shared<SimpleDataSource<int>>(data_provider{});

    SimplePipeline<int, std::string> pipeline(in_pipe, capacity_per_pipe);
    pipeline.addDataSource(data_source);
    pipeline.addDataFilter(std::function<int(int)>{plus_one})
            .addDataFilter(std::function<int(int)>{mul_two})
            .addDataFilter(std::function<std::string(int)>(print));

    cout << fixed << setprecision(1);
    auto out_pipe = pipeline.getSinkPipe();
    std::string output;
    pipeline.start();
    while (true) {
        auto start_time = system_clock::now();
        out_pipe->pop(output);
        auto end_time = system_clock::now();
        cout << output << ": " << duration<double>(end_time-start_time).count() << "s" << endl;
    }
}
