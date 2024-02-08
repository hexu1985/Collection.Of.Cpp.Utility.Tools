#include <iostream>
#include <functional>
#include <future>
#include <thread>
#include <chrono>
#include <cstdlib>

void thread_set_promise(std::promise<int>& promiseObj) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    promiseObj.set_value(100); // set_value后，future变为就绪。
}

int main() {
    std::promise<int> promiseObj;
    std::future<int> futureObj = promiseObj.get_future();
    std::thread t(&thread_set_promise, std::ref(promiseObj)); 
    // 采用std::ref引用传值
    std::cout << futureObj.get() << std::endl; // 会阻塞

    t.join();
    return 0;
}
