#include <unistd.h>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/thread/thread.hpp>

namespace bip = boost::interprocess;
using namespace std::literals;

bip::named_mutex global_mutex(bip::open_or_create,"mtx");

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
    boost::lock_guard<bip::named_mutex> lock(global_mutex);//读锁定
    for (int i = 0; i < print_times; i++) {
        print_pid(pid);
        std::this_thread::sleep_for(500ms);
    }

    return 0;
}

