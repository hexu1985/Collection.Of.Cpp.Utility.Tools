// 使用示例
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "thread_name.hpp"

void worker(int id) {
    // 设置当前线程名
    current_thread::set_thread_name("Worker-" + std::to_string(id));
    std::cout << "Thread " << id << " name: " 
              << current_thread::get_thread_name() << " start" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // 获取并使用线程名
    std::cout << "Thread " << id << " name: " 
              << current_thread::get_thread_name() << " finish" << std::endl;
}

int main() {
    current_thread::set_thread_name("MainThread");
    std::cout << "Main thread: " 
              << current_thread::get_thread_name() << std::endl;
    
    std::vector<std::thread> threads;
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(worker, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // 主线程的名称不受影响
    std::cout << "Main thread still: " 
              << current_thread::get_thread_name() << std::endl;
    
    return 0;
}
