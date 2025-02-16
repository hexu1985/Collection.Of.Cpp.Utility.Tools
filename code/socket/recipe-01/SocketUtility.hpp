#pragma once

#include <string>

enum class ByteOrder {
    unknown = 0,
    big_endian = 1,
    little_endian = 2,
};

ByteOrder check_byte_order();

std::string Gethostname();
std::string Gethostbyname(const std::string& hostname);
