#include <thread>
#include <chrono>
#include <iostream>

#include "limitedsize_queue.hpp"

void put_id(limitedsize_queue<int>& queue) {
    int i = 0;
    while (true) {
        i = i + 1;
        std::cout << "添加数据 " << i << std::endl;
        queue.push(i, queue_push_policy::drop_queue_front_item);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void get_id(int thread_id, limitedsize_queue<int>& queue) {
    while (true) {
        int i;
        queue.pop(i);
        std::cout << "线程: " << thread_id << " 取值 " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    limitedsize_queue<int> id_queue(5);
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
