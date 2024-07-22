#include <unistd.h>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <mutex>
#include "interprocess_mutex.hpp"

using namespace std::literals;

InterprocessMutex global_mutex("mtx");

void print_pid(int pid=-1) {
    if (pid < 0) {
        std::cout << "process #" << getpid() << std::endl;
    } else {
        std::cout << "process #" << pid << std::endl;
    }
}

int main(int argc, char* argv[]) {
    int pid = -1;
    if (argc > 1) {
        pid = std::stoi(argv[1]);
    }

    int print_times = 10;
    std::lock_guard<InterprocessMutex> lock(global_mutex);//读锁定
    for (int i = 0; i < print_times; i++) {
        print_pid(pid);
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}

