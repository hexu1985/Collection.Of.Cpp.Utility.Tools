#include "socket.hpp"

#include <netinet/in.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace unpsock;

static constexpr int   TTL      = 64;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage : " << argv[0] << " <GroupIP> <PORT>\n";
        return 1;
    }

    const std::string group_ip = argv[1];
    const uint16_t port = static_cast<uint16_t>(std::stoi(argv[2]));

    Socket send_sock(Family::INET, AddrType::DGRAM);

    send_sock.setsockopt<unsigned char>(
            IPPROTO_IP, IP_MULTICAST_TTL,
            static_cast<unsigned char>(TTL));

    Address mul_addr = Address::from_ip(group_ip, port, Family::INET);

    std::ifstream fp("news.txt", std::ios::binary);
    if (!fp) {
        std::cerr << "fopen() error\n";
        return 1;
    }

    std::string buf;
    while (std::getline(fp, buf)) {
        buf += '\n';
        send_sock.sendto(buf, mul_addr);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    send_sock.close();

    return 0;
}
