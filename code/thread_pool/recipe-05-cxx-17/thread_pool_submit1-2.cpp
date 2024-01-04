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

template <typename T>
bool is_done(std::future<T>& f) {
    return f.wait_until(std::chrono::system_clock::now()) == std::future_status::ready;
}


int main() {
    thread_pool t{5};

    auto task1 = t.submit(spider, 1);
    auto task2 = t.submit(spider, 2);
    auto task3 = t.submit(spider, 3);

    std::cout << std::boolalpha;
    std::cout << "task1: " << is_done(task1) << std::endl;
    std::cout << "task2: " << is_done(task2) << std::endl;
    std::cout << "task3: " << is_done(task3) << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    std::cout << "task1: " << is_done(task1) << std::endl;
    std::cout << "task2: " << is_done(task2) << std::endl;
    std::cout << "task3: " << is_done(task3) << std::endl;

    std::cout << task1.get() << std::endl;

    char c;
    std::cin >> c;
    return 0;
}

