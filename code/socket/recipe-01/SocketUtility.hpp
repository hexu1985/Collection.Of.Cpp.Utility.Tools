#pragma once

enum class ByteOrder {
    unknown = 0,
    big_endian = 1,
    little_endian = 2,
};

ByteOrder check_byte_order();
