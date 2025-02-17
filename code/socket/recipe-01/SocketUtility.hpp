#pragma once

#include <string>
#include <cstdint>

enum class ByteOrder {
    unknown = 0,
    big_endian = 1,
    little_endian = 2,
};

ByteOrder check_byte_order();

std::string Gethostname();
std::string Gethostbyname(const std::string& hostname);

uint32_t Inet_aton(const std::string& ip_string);
std::string Inet_ntoa(uint32_t packed_ip);

