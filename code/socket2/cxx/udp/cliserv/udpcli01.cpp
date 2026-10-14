#include "socket.hpp"

#include <iostream>
#include <string>
#include <string_view>

using namespace unpsock;

const int SERV_PORT = 9877;   // 对应 C 代码中的 SERV_PORT
const int MAXLINE = 4096;     // 对应 MAXLINE

void dg_cli(std::istream& in, Socket& sock, const Address& servaddr)
{
    std::string sendline;
    while (std::getline(in, sendline)) {
        sendline += '\n';
        sock.sendto(sendline, servaddr);

        auto [recvline, _] = sock.recvfrom(MAXLINE);
        std::cout << std::string_view(reinterpret_cast<const char*>(recvline.data()), recvline.size()) << std::ends;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: udpcli <IPaddress>" << std::endl;
        return 1;
    }

    auto servaddr = Address::from_ip(argv[1], SERV_PORT);

    Socket sock(Family::INET, AddrType::DGRAM);

    dg_cli(std::cin, sock, servaddr);

    sock.close();
    return 0;
}
