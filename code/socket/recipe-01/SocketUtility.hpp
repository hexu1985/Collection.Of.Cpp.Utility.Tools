#pragma once

#include <string>
#include <arpa/inet.h>

enum class ByteOrder {
    unknown = 0,
    big_endian = 1,
    little_endian = 2,
};

ByteOrder check_byte_order();

std::string Gethostname();
std::string Gethostbyname(const std::string& hostname);

in_addr_t Inet_aton(const std::string& ip_string);
std::string Inet_ntoa(in_addr_t packed_ip);

