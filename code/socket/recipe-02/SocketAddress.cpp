#include "SocketAddress.hpp"
#include "SocketUtility.hpp"

#include <string.h>
#include <sstream>
#include <iostream>

SocketAddress::SocketAddress() {
    memset(&addr_, 0x0, sizeof(addr_));
    addr_len_ = sizeof(addr_);
}

SocketAddress::SocketAddress(struct sockaddr* addr, socklen_t addr_len) {
    memset(&addr_, 0x0, sizeof(addr_));
    memcpy(&addr_, addr, addr_len);
    addr_len_ = addr_len;
}

void SocketAddress::clean() {
    memset(&addr_, 0x0, sizeof(addr_));
    addr_len_ = sizeof(addr_);
}

std::string SocketAddress::to_string() const {
    auto address = sock_ntop(reinterpret_cast<const struct sockaddr*>(&addr_), addr_len_);
    std::ostringstream os;
    os << "(" << std::get<0>(address) << ", " << std::get<1>(address) << ")";
    return os.str();
}

std::ostream& operator<< (std::ostream& out, const SocketAddress& address) {
    out << address.to_string();
    return out;
}
