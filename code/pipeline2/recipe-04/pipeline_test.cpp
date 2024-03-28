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
    Pipe<int> in_pipe = make_pipe<int>();
    auto data_source = make_simple_data_source<int>(data_provider{});

    Pipeline<int, std::string> pipeline(in_pipe);
    pipeline.addDataSource(data_source);
    pipeline.addDataFilter(make_simple_data_filter<int, int>(plus_one))
            .addDataFilter(make_simple_data_filter<int, int>(mul_two))
            .addDataFilter(make_simple_data_filter<int, std::string>(print));

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
