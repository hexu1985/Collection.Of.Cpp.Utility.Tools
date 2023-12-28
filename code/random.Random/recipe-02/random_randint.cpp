#include "random.hpp"
#include <iostream>

int main() {
    std::cout << "[1, 100]: ";

    for (int i = 0; i < 3; i++) {
        std::cout << randint(1, 100) << " ";
    }

    std::cout << "\n[-5, 5]:";
    for (int i = 0; i < 3; i++) {
        std::cout << randint(-5, 5) << " ";
    }
    std::cout << std::endl;
}
