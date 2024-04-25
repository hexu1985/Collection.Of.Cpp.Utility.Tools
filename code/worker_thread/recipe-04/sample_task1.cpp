#include <iostream>
#include "task.hpp"

void print() {
    std::cout << "hello world!" << std::endl;
}

int main() {
    Task task(print);
    task.Run();

    Task task2(std::move(task));
    task.Run();
    task2.Run();

    return 0;
}
