#include <iostream>
#include <functional>
#include <future>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <tuple>

using ret_type = std::tuple<bool, int>;

void thread_set_promise(std::promise<ret_type>& promiseObj) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    promiseObj.set_value(std::make_tuple(false,100)); // set_value后，future变为就绪。
}

int main() {
    std::promise<ret_type> promiseObj;
    std::future<ret_type> futureObj = promiseObj.get_future();
    std::thread t(&thread_set_promise, std::ref(promiseObj)); 

    bool flag;
    int value;

    std::tie(flag, value) = futureObj.get();

    // 采用std::ref引用传值
    std::cout << std::boolalpha << flag << ", " << value << std::endl; // 会阻塞

    t.join();
    return 0;
}
