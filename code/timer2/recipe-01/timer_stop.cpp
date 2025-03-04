#include "timer.hpp"
#include "print_message.hpp"
#include <chrono>
#include <thread>

void say_hello() {
    print_message("Hello, World!");
}

int main() {
    auto timer = Timer{say_hello, std::chrono::seconds{5}};
    print_message("Timer started, but we will cancel it.");
    timer.stop();
    if (!timer.isRunning()) {
        print_message("Timer cancelled.");
    }
    std::this_thread::sleep_for(std::chrono::seconds{10});
    print_message("Program exit.");
}

