#include "SocketAddress.hpp"

#include <string.h>

SocketAddress::SocketAddress() {
    memset(&addr_, 0x0, sizeof(addr_));
    addr_len_ = 0;
}

SocketAddress::SocketAddress(struct sockaddr* addr, socklen_t addr_len) {
    memset(&addr_, 0x0, sizeof(addr_));
    memcpy(&addr_, addr, addr_len);
    addr_len_ = addr_len;
}
