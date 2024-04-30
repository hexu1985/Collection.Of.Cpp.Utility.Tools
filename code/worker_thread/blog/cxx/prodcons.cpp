#include <iostream>
#include <thread>
#include <chrono>

#include "random.hpp"
#include "worker_thread.hpp"

void mul_two(int i) {
    std::cout << "mul_two(" << i << ") is " << i*2 << std::endl;
}

void consume(int i) {
    mul_two(i);
    std::this_thread::sleep_for(std::chrono::seconds(randint(2, 5)));
}

void producer(std::shared_ptr<TaskQueue> queue, int loops) {
    for (int i = 0; i < loops; i++) {
        queue->PushTask(&consume, i);
        std::this_thread::sleep_for(std::chrono::seconds(randint(1, 3)));
    }
}

int main() {
    int nloops = randint(2, 5);

    WorkerThread myworker("worker");
    myworker.Start();

    std::thread myproducer(&producer, myworker.GetTaskQueue(), nloops);
    myproducer.join();

    myworker.Stop();
    std::cout << "all DONE" << std::endl;

    return 0;
}
