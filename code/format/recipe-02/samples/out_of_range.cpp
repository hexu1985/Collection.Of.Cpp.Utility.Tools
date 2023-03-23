#include <iostream>

#include "format.hpp"

using namespace util;

int main() {
    std::cout << Format("{0} {2}", "hello", "world") << std::endl;
    return 0;
}

