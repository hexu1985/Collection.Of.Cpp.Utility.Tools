#include "timer.hpp"
#include "print_message.hpp"
#include <chrono>
#include <thread>

void say_hello() {
    print_message("Hello, World!");
}

int main() {
    auto timer = Timer{say_hello, std::chrono::seconds{1}, Timer::repeat};
    print_message("Timer started, but we will cancel it after 3s.");
    std::this_thread::sleep_for(std::chrono::seconds{3});
    print_message("Now we stop the timer.");
    timer.stop();
    print_message("Wait for 3s, and check timer status.");
    std::this_thread::sleep_for(std::chrono::seconds{3});
    if (!timer.isRunning()) {
        print_message("Timer cancelled.");
    }
    print_message("Program exit.");

    return 0;
}

