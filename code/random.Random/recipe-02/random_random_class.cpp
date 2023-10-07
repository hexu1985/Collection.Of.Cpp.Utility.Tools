#include "Random.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>

int main() {
    std::cout << "Default initializiation:\n" << std::endl;

    auto r1 = random_::Random();
    auto r2 = random_::Random();

    std::cout << std::fixed << std::setprecision(3);
    for (int i = 0; i < 3; i++) {
        std::cout << r1.random() << "  " << r2.random() << std::endl;
    }

    std::cout << "\nSame seed:\n" << std::endl;

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    r1 = random_::Random(seed);
    r2 = random_::Random(seed);

    for (int i = 0; i < 3; i++) {
        std::cout << r1.random() << "  " << r2.random() << std::endl;
    }
}

