#include "socket.hpp"
#include <iostream>

void print_machine_info() {
    // 1. 获取本机主机名
    std::string hostname = Socket::host::local_hostname();

    // 2. 解析主机名，拿到所有地址（IPv4 + IPv6）
    //    内部走 getaddrinfo，替代已废弃的 gethostbyname
    auto addrs = Socket::Address::resolve_all(hostname, 0);

    // 3. 输出
    std::cout << "Host name: " << hostname << "\n";
    for (const auto& addr : addrs) {
        std::cout << "IP address: " << addr.ip() << "\n";
    }
}

int main() {
    print_machine_info();
    return 0;
}
