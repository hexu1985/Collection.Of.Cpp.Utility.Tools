#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>

#include "limitedsize_queue.hpp"

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

void put_id(limitedsize_queue<int>& queue) {
    int i = 0;
    while (true) {
        i = i + 1;
        std::cout << std::chrono::system_clock::now() << ":" << "添加数据 " << i << std::endl;
        queue.push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void get_id(int thread_id, limitedsize_queue<int>& queue) {
    while (true) {
        int i;
        if (queue.pop(i, std::chrono::seconds(1))) {
            std::cout << std::chrono::system_clock::now() << ":" << "线程: " << thread_id << " 取值 " << i << std::endl;
        } else {
            std::cout << std::chrono::system_clock::now() << ":" << "线程: " << thread_id << " 取值超时 " << std::endl;
        }
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
