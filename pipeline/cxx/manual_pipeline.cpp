#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>

#include "threadsafe_queue.hpp"

using namespace std;
using namespace std::chrono;
using data_queue = std::shared_ptr<threadsafe_queue<int>>;

void data_provider(data_queue q) {
    for (int i = 0; i < 5; i++) {
        q->push(i);
    }
}

void plus_one(data_queue inq, data_queue outq) {
    while (true) {
        int x;
        inq->wait_and_pop(x);
        this_thread::sleep_for(milliseconds(500));
        outq->push(x);
    }
}

void mul_two(data_queue inq, data_queue outq) {
    while (true) {
        int x;
        inq->wait_and_pop(x);
        this_thread::sleep_for(milliseconds(500));
        outq->push(x);
    }
}

int main() {
    std::vector<data_queue> queues;
    for (int i = 0; i < 3; i++) {
        queues.push_back(data_queue(new threadsafe_queue<int>));
    }

    std::vector<std::thread> processes;
    processes.push_back(std::thread(&data_provider, queues[0]));
    processes.push_back(std::thread(&plus_one, queues[0], queues[1]));
    processes.push_back(std::thread(&mul_two, queues[1], queues[2]));

    cout << fixed << setprecision(1);
    int output;
    while (true) {
        auto start_time = system_clock::now();
        queues[2]->wait_and_pop(output);
        auto end_time = system_clock::now();
        cout << output << ": " << duration<double>(end_time-start_time).count() << "s" << endl;
    }
}

