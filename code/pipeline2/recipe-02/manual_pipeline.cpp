#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <string>

#include "limitedsize_queue.hpp"

using namespace std;
using namespace std::chrono;

template <typename T>
using DataQueue = std::shared_ptr<limitedsize_queue<T>>;

void data_provider(DataQueue<int> q) {
    for (int i = 0; i < 5; i++) {
        q->push(i);
    }
}

void plus_one(DataQueue<int> inq, DataQueue<int> outq) {
    while (true) {
        int x;
        inq->pop(x);
        this_thread::sleep_for(milliseconds(500));
        outq->push(x);
    }
}

void mul_two(DataQueue<int> inq, DataQueue<int> outq) {
    while (true) {
        int x;
        inq->pop(x);
        this_thread::sleep_for(milliseconds(500));
        outq->push(x);
    }
}

void print(DataQueue<int> inq, DataQueue<std::string> outq) {
    while (true) {
        int x;
        inq->pop(x);
        this_thread::sleep_for(milliseconds(500));
        outq->push(std::to_string(x));
    }
}

int main() {
    std::vector<DataQueue<int>> queues;
    for (int i = 0; i < 3; i++) {
        queues.push_back(DataQueue<int>(new limitedsize_queue<int>));
    }
    DataQueue<std::string> output_queue = DataQueue<std::string>(new limitedsize_queue<std::string>);

    std::vector<std::thread> processes;
    processes.push_back(std::thread(&data_provider, queues[0]));
    processes.push_back(std::thread(&plus_one, queues[0], queues[1]));
    processes.push_back(std::thread(&mul_two, queues[1], queues[2]));
    processes.push_back(std::thread(&print, queues[2], output_queue));

    cout << fixed << setprecision(1);
    std::string output;
    while (true) {
        auto start_time = system_clock::now();
        output_queue->pop(output);
        auto end_time = system_clock::now();
        cout << output << ": " << duration<double>(end_time-start_time).count() << "s" << endl;
    }
}

