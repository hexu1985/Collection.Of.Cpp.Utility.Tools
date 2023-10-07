#include "Random.hpp"
#include <iomanip>
#include <iostream>

int main() {
    random_::seed(1);
    std::cout << std::fixed << std::setprecision(3);
    for (int i = 0; i < 5; i++) {
        std::cout << random_::random() << " ";
    }
    std::cout << std::endl;
    
    return 0;
}

