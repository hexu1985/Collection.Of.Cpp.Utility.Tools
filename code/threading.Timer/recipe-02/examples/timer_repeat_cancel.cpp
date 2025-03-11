#include "timer.hpp"
#include "print_message.hpp"
#include <chrono>
#include <thread>
#include <atomic>
#include <string>

std::atomic<int> count{0};
void repeat_hello() {
    count++;
    print_message(std::to_string(count));
    print_message("Hello, World!");
}

int main() {
    auto timer = Timer{repeat_hello, std::chrono::seconds{2}, Timer::repeat};
    timer.start();
    print_message("Repeating timer started...");
    while (timer.isActive()) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
        if (count >= 3) {
            print_message("Now we will cancel it.");
            timer.cancel();
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds{2});
    print_message("Timer cancelled.");
}
