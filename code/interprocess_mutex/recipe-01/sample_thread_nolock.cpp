// mutex example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <chrono>

void print_block (int n, char c) {
    // critical section (exclusive access to std::cout signaled by locking mtx):
    for (int i=0; i<n; ++i) { std::cout << c;  std::this_thread::sleep_for(std::chrono::milliseconds(20)); }
    std::cout << '\n';
}

int main ()
{
    std::thread th1 (print_block,50,'*');
    std::thread th2 (print_block,50,'$');

    th1.join();
    th2.join();

    return 0;
}
