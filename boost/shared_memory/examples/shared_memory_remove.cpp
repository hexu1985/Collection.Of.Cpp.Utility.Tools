#include <boost/interprocess/shared_memory_object.hpp>
#include <iostream>

using namespace boost::interprocess;

int main(int argc, char* argv[]) {
    const char* name = "Boost";
    if (argc > 1) {
        name = argv[1];
    }

    bool removed = shared_memory_object::remove(name);
    std::cout << std::boolalpha << removed << '\n'; 

    return 0;
}
