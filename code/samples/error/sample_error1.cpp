#include <iostream>
#include "error.hpp"

int main()
{
    try {
        mini_util::error("invalid argument %d, %d, %s", 12, 34, "hello");
    } catch (const std::runtime_error &e) {
        std::cout << "catch runtime error: " << e.what() << std::endl;
    }
}

