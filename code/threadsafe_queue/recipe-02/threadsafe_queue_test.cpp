#include <thread>
#include <chrono>
#include <iostream>

#include "threadsafe_queue.hpp"

void put_id(threadsafe_queue<int>& queue) {
    int i = 0;
    while (true) {
        i = i + 1;
        std::cout << "添加数据 " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue.push(i);
    }
}

void get_id(int thread_id, threadsafe_queue<int>& queue) {
    while (true) {
        int i;
        queue.wait_and_pop(i);
        std::cout << "线程: " << thread_id << " 取值 " << i << std::endl;
    }
}

int main() {
    threadsafe_queue<int> id_queue;
    auto Th1 = std::thread(put_id, std::ref(id_queue));

    auto Th2 = std::thread(get_id, 2, std::ref(id_queue));
    auto Th3 = std::thread(get_id, 3, std::ref(id_queue));
    auto Th5 = std::thread(get_id, 4, std::ref(id_queue));
    auto Th4 = std::thread(get_id, 5, std::ref(id_queue));

    Th2.join();
    Th1.join();

    Th3.join();
    Th4.join();
    Th5.join();
}
