#include <iostream>

#include "format.hpp"

using namespace util;

int main() {
    std::cout << Format("{{}} {} {}", "hello", "world") << std::endl;
    std::cout << Format("{} {}", "hello", "world") << std::endl;
    std::cout << Format("{0} {1}", "hello", "world") << std::endl;
    std::cout << Format("{1} {0} {1}", "hello", "world") << std::endl;
    return 0;
}
