#include "timer.hpp"
#include "print_message.hpp"
#include <chrono>
#include <thread>

void say_hello() {
    print_message("Hello, World!");
}

int main() {
    auto timer = Timer{say_hello, std::chrono::seconds{3}};
    print_message("Timer started, waiting for it to trigger...");
    while (timer.isRunning()) {
        print_message("Timer is running...");
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
    print_message("Timer is not running.");
}
