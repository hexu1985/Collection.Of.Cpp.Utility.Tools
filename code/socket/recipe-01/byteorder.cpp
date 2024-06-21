#include <iostream>
#include "SocketUtility.hpp"

using namespace std;

int main(int argc, char **argv)
{
    auto endian = check_byte_order();
    if (endian == ByteOrder::big_endian) {
        std::cout << "big-endian\n";
    } else if (endian == ByteOrder::little_endian) {
        std::cout << "little-endian\n";
    } else {
        std::cout << "unknown\n";
    }

    return 0;
}
