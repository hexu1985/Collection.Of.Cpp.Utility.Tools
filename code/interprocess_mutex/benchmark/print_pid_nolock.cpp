#include <unistd.h>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>

using namespace std::literals;

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
    for (int i = 0; i < print_times; i++) {
        print_pid(pid);
        std::this_thread::sleep_for(500ms);
    }

    return 0;
}

