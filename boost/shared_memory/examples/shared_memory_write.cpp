#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

using namespace boost::interprocess;

int main(int argc, char* argv[]) {
    const char* name = "Boost";
    if (argc > 1) {
        name = argv[1];
    }

    shared_memory_object shdmem{open_only, name, read_write};
    shdmem.truncate(1024);
    std::cout << shdmem.get_name() << '\n';
    offset_t size;
    if (shdmem.get_size(size))
        std::cout << size << '\n';


    mapped_region region{shdmem, read_write};
    char* ptr = static_cast<char*>(region.get_address());

    std::string data = "test data ";
    for (int i = 0; i < size; i++) {
        *ptr++ = data[i%data.size()];
    }

    return 0;
}
