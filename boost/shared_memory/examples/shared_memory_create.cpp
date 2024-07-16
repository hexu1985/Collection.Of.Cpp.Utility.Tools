#include <boost/interprocess/shared_memory_object.hpp>
#include <iostream>

using namespace boost::interprocess;

int main(int argc, char* argv[]) {
    const char* name = "Boost";
    if (argc > 1) {
        name = argv[1];
    }

    shared_memory_object shdmem{create_only, name, read_write};
    shdmem.truncate(1024);
    std::cout << shdmem.get_name() << '\n';
    offset_t size;
    if (shdmem.get_size(size))
        std::cout << size << '\n';

    return 0;
}
