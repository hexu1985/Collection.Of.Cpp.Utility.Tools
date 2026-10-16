#include "socket.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdlib>

using namespace unpsock;

static constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage : " << argv[0] << " <GroupIP> <PORT>\n";
        return 1;
    }

    const std::string group_ip = argv[1];
    const uint16_t port = static_cast<uint16_t>(std::stoi(argv[2]));

    Socket recv_sock(Family::INET, AddrType::DGRAM);

    recv_sock.set_reuse_addr(true);

    recv_sock.bind(Address::any(port, Family::INET));

    struct ip_mreq mreq{};
    if (::inet_pton(AF_INET, group_ip.c_str(), &mreq.imr_multiaddr) != 1) {
        std::cerr << "invalid group ip: " << group_ip << "\n";
        return 1;
    }
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    recv_sock.setsockopt<struct ip_mreq>(
            IPPROTO_IP, IP_ADD_MEMBERSHIP, mreq);

    for (;;) {
        std::error_code ec;
        auto [data, peer] = recv_sock.recvfrom(BUF_SIZE - 1, ec);

        if (ec) {
            std::cerr << "recvfrom error: " << ec.message() << "\n";
            break;
        }

        std::cout.write(reinterpret_cast<const char*>(data.data()),
                static_cast<std::streamsize>(data.size()));
        std::cout.flush();
    }

    return 0;
}
