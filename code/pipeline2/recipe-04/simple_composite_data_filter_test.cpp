#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include "simple_composite_data_filter.hpp"
#include "simple_data_source.hpp"
#include "simple_data_filter.hpp"
#include "simple_data_sink.hpp"
#include "pipeline.hpp"

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

void data_receiver(std::string& data) {
    std::cout << "receive data: " << data << std::endl;
}

int main() {
    const size_t capacity_per_pipe = 1;
    auto composite_data_filter = make_simple_composite_data_filter<int, std::string>(capacity_per_pipe);
    composite_data_filter->addDataFilter(std::function<int(int)>{plus_one});
    composite_data_filter->addDataFilter(std::function<int(int)>{mul_two});
    composite_data_filter->addDataFilter(std::function<std::string(int)>(print));

    Pipeline<int, std::string> pipeline(make_pipe<int>(capacity_per_pipe), capacity_per_pipe);
    pipeline.addDataSource(make_simple_data_source<int>(data_provider{}))
            .addDataFilter(to_composite_data_filter(composite_data_filter))
            .addDataSink(make_simple_data_sink<std::string>(data_receiver));
    cout << fixed << setprecision(1);
    std::string output;
    pipeline.start();
    std::cin.get();
    pipeline.stop();
}
