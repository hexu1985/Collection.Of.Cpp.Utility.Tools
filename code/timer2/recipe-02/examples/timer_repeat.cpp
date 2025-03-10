#include "timer.hpp"
#include "print_message.hpp"
#include <chrono>
#include <thread>

void repeat_hello() {
    print_message("Hello, World!");
}

int main() {
    auto timer = Timer{repeat_hello, std::chrono::seconds{2}, Timer::repeat};
    print_message("Repeating timer started...");
    while (timer.isRunning()) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}
