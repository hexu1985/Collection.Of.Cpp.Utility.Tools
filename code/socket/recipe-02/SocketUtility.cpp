#include "SocketUtility.hpp"
#include <cstdint>

ByteOrder check_byte_order() {
    union {
        uint16_t s;
        char     c[sizeof(uint16_t)];
    } un;

    un.s = 0x0102;
    if (un.c[0] == 1 && un.c[1] == 2)
        return ByteOrder::big_endian;
    else if (un.c[0] == 2 && un.c[1] == 1)
        return ByteOrder::little_endian;
    else
        return ByteOrder::unknown;
}
