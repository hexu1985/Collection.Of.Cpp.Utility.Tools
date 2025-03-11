#include "timer.hpp"
#include "print_message.hpp"
#include <chrono>
#include <thread>

void repeat_hello(Timer timer) {
    static int count{0};
    ++count;
    print_message(std::to_string(count));
    print_message("Hello, World!");
    if (count >= 5) {
        print_message("cancel myself");
        timer.cancel();
    }
}

int main() {
    Timer timer;
    timer.start(std::bind(&repeat_hello, timer), std::chrono::seconds{2}, Timer::repeat);
    print_message("Repeating timer started...");
    while (timer.isActive()) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
    std::this_thread::sleep_for(std::chrono::seconds{2});
    print_message("Timer cancelled.");
}
