#include <iostream>
#include <thread>
#include <mutex>

std::once_flag flag;
bool is_init = false;

void initialize() {
    is_init = true;
    std::cout << "Initialized only once!" << std::endl;
}

void worker() {
    std::call_once(flag, initialize);
    if (is_init) {
        std::cout << "is init" << std::endl;
    } else {
        std::cout << "not init" << std::endl;
    }
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);
    std::thread t3(worker);
    
    t1.join(); t2.join(); t3.join();
    // 输出: "Initialized only once!" (只输出一次)
}
