#include <cstdlib>
#include <iostream>
#include <string>

std::string say_hello() { return std::string("Hello, CMake world!"); }

int main() {
#ifndef NDEBUG
    std::cout << "hello debug" << std::endl;
#else
    std::cout << "hello release" << std::endl;
#endif
    std::cout << say_hello() << std::endl;
    return EXIT_SUCCESS;
}
