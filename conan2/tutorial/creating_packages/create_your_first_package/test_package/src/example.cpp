#include "hello.h"
#include <vector>
#include <string>

int main() {
    hello();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    hello_print_vector(vec);
}
