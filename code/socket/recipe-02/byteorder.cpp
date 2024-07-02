#include <stdio.h>
#include "SocketUtility.hpp"

using namespace std;

int main(int argc, char **argv)
{
    auto endian = check_byte_order();
    if (endian == ByteOrder::big_endian) {
        printf("big-endian\n");
    } else if (endian == ByteOrder::little_endian) {
        printf("little-endian\n");
    } else {
        printf("unknown\n");
    }

    return 0;
}
