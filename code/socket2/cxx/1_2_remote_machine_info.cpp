#include "socket.hpp"

#include <iostream>
#include <string>

void get_remote_machine_info() {
    const std::string remote_host = "www.python.org";

    // 一步完成 getaddrinfo + 遍历 + 转换
    // 默认 Family::UNSPEC（IPv4/IPv6 都接受），AddrType::STREAM（TCP）
    auto addrs = Socket::Address::resolve_all(remote_host, 0);

    for (const auto& addr : addrs) {
        const char* version = addr.is_ipv4() ? "IPv4"
            : addr.is_ipv6() ? "IPv6"
            : "Unknown";
        std::cout << "IP address of " << remote_host
            << " (" << version << "): "
            << addr.ip() << "\n";
    }
}

int main() {
    get_remote_machine_info();
    return 0;
}
