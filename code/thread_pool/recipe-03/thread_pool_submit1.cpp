#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include "thread_pool.hpp"

int spider(int page) {
    std::this_thread::sleep_for(std::chrono::seconds(page));
    std::cout << "crawl task" << page << " finished" << std::endl;
    return page;
}

int main() {
    thread_pool t{5};

    auto task1 = t.submit(std::bind(spider, 1));
    auto task2 = t.submit(std::bind(spider, 2));
    auto task3 = t.submit(std::bind(spider, 3));

    std::cout << std::boolalpha;
    std::cout << "task1: " << (task1.wait_until(std::chrono::system_clock::now()) == std::future_status::ready) << std::endl;
    std::cout << "task2: " << (task2.wait_until(std::chrono::system_clock::now()) == std::future_status::ready) << std::endl;
    std::cout << "task3: " << (task3.wait_until(std::chrono::system_clock::now()) == std::future_status::ready) << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    std::cout << "task1: " << (task1.wait_until(std::chrono::system_clock::now()) == std::future_status::ready) << std::endl;
    std::cout << "task2: " << (task2.wait_until(std::chrono::system_clock::now()) == std::future_status::ready) << std::endl;
    std::cout << "task3: " << (task3.wait_until(std::chrono::system_clock::now()) == std::future_status::ready) << std::endl;

    std::cout << task1.get() << std::endl;

    char c;
    std::cin >> c;
    return 0;
}

