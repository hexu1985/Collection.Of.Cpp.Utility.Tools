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

std::string strftime(const char* format, const std::chrono::time_point<std::chrono::system_clock>& tp) {
    time_t rawtime = std::chrono::system_clock::to_time_t(tp);
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), format, localtime(&rawtime));
    return std::string(mbstr);
}

std::ostream& operator<<(std::ostream& out, const std::chrono::time_point<std::chrono::system_clock>& tp) {
    auto cs = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()).count() % 1000000;
    out << strftime("%Y-%m-%d %H:%M:%S", tp) << '.' << std::setfill('0') << std::setw(6) << cs << std::setfill(' ');
    return out;
}

void data_receiver(std::string& data) {
    std::cout << std::chrono::system_clock::now() << ": receive data: " << data << std::endl;
}

int main() {
    SimplePipeline<int, std::string> pipeline;
    pipeline.add_data_source(std::function<bool(int&)>{data_provider{}})
            .add_data_filter(std::function<int(int)>{plus_one})
            .add_data_filter(std::function<int(int)>{mul_two})
            .add_data_filter(std::function<std::string(int)>{print})
            .add_data_sink(std::function<void(std::string&)>(data_receiver));
    cout << fixed << setprecision(1);
    std::string output;
    pipeline.start();
    std::cin.get();
    pipeline.stop();
}
