#include <arpa/inet.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "SocketUtility.hpp"

void convert_ip4_address() {
    std::vector<std::string> addr_list = {"127.0.0.1", "192.168.0.1"};
    for (const auto& ip_addr : addr_list) {
        auto packed_ip_addr = Inet_aton(ip_addr);
        auto unpacked_ip_addr = Inet_ntoa(packed_ip_addr);
        std::cout << "IP Address: " << ip_addr << " => Packed: "
            << std::hex << std::setw(2) << std::setfill('0')
            << htonl(packed_ip_addr) << ", Unpacked: " 
            << unpacked_ip_addr << "\n";
    }
}

int main(int argc, char **argv)
{
    convert_ip4_address();
    return 0;
}


