#include <unistd.h>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::literals;

void print_pid() {
    std::cout << "process #" << getpid() << std::endl;
}

int main(int argc, char* argv[]) {
    int print_times = 10;
    for (int i = 0; i < print_times; i++) {
        print_pid();
        std::this_thread::sleep_for(500ms);
    }

    return 0;
}

