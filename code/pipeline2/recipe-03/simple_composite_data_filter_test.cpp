#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include "simple_data_source.hpp"
#include "simple_composite_data_filter.hpp"

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
    Pipe<std::string> out_pipe = make_pipe<std::string>();
    SimpleDataSource<int> data_source(data_provider{});
    data_source.setOutPipe(in_pipe);

    SimpleCompositeDataFilter<int, std::string> composite_data_filter;
    composite_data_filter.addDataFilter(std::function<int(int)>{plus_one})
            .addDataFilter(std::function<int(int)>{mul_two})
            .addDataFilter(std::function<std::string(int)>(print));
    composite_data_filter.setInPipeAny(in_pipe);
    composite_data_filter.setOutPipeAny(out_pipe);

    cout << fixed << setprecision(1);
    std::string output;
    composite_data_filter.start();
    data_source.start();
    while (true) {
        auto start_time = system_clock::now();
        out_pipe->pop(output);
        auto end_time = system_clock::now();
        cout << output << ": " << duration<double>(end_time-start_time).count() << "s" << endl;
    }
}
