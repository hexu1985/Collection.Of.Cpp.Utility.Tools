#include <iostream>
#include "limitedsize_queue.hpp"

int main() {
    limitedsize_queue<int> q;
    for (int i = 0; i < 5; i++) {
        q.push(i);
    }

    int value;
    while (!q.empty()) {
        q.pop(value);
        std::cout << value << ' ';
    }
    std::cout << std::endl;
    return 0;
}
