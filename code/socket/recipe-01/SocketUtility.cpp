#include "SocketUtility.hpp"
#include "SocketException.hpp"
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdint>

#if __cplusplus >= 202002L
#include <format>
using std::format;
#else
#define FMT_HEADER_ONLY
#include "fmt/format.h"
using fmt::format;
#endif

namespace {

std::string Inet_ntop(int family, const void *addrptr) {
    char buf[INET6_ADDRSTRLEN] = {0};
    const char* ptr;
    if ((ptr = inet_ntop(family, addrptr, buf, sizeof(buf))) == NULL) {
        throw OSError(errno, "Inet_ntop() error");
    }
    return ptr;
}

}   // namespace

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

std::string Gethostname() {
    constexpr int max_host_name_len = 128;
    char host_name[max_host_name_len] = {0};
    if (gethostname(host_name, max_host_name_len) < 0) {
        throw OSError(errno, "Gethostname() error");
    }
    return host_name;
}

std::string Gethostbyname(const std::string& hostname) {
	struct hostent* hptr = gethostbyname(hostname.c_str());
    if (!hptr) {
        throw HError(h_errno, format("Gethostbyname error for {}", hostname));
    }

    return Inet_ntop(AF_INET, (struct in_addr *)hptr->h_addr_list[0]);
}

uint32_t Inet_aton(const std::string& ip_string) {
    in_addr in;
    if (!inet_aton(ip_string.c_str(), &in)) {
        throw OSError(format("Inet_aton error for {}", ip_string));
    }
    return in.s_addr;
}

std::string Inet_ntoa(uint32_t packed_ip) {
    in_addr in;
    in.s_addr = packed_ip;
    return inet_ntoa(in);
}

