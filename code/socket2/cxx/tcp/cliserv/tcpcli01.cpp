#include "socket.hpp"

#include <iostream>
#include <string>

using namespace unpsock;

const int SERV_PORT = 9877;   // 对应 C 代码中的 SERV_PORT
const int MAXLINE = 4096;     // 对应 MAXLINE

inline std::string readline(Socket& sock, size_t maxlen,
                                     std::error_code& ec) noexcept
{
    ec.clear();
    std::string out;

    out.reserve(maxlen);   // 最多 maxlen 字节，一次分配到位

    for (size_t n = 0; n < maxlen; ++n) {
        uint8_t c = 0;
        size_t got = sock.recv(&c, 1, ec);

        if (ec) {
            out.clear();
            return out;
        }
        if (got == 0) {
            return out;
        }

        out.push_back(c);
        if (c == '\n') break;
    }

    return out;
}

// 异常版本
inline std::string readline(Socket& sock, size_t maxlen) {
    std::error_code ec;
    auto v = readline(sock, maxlen, ec);
    if (ec) {
        throw SocketError(ec, "readline failed");
    }
    return v;
}

// 对应 C 中的 str_cli
void str_cli(std::istream& in, Socket& sock)
{
    std::string sendline;
    while (std::getline(in, sendline)) {
        sendline += '\n';
        sock.sendall(sendline);

        // 读回一行
        auto recvline = readline(sock, MAXLINE);
        std::cout << sendline << std::ends;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: tcpcli <IPaddress>" << std::endl;
        return 1;
    }

    Socket sock(Family::INET, AddrType::STREAM);
    auto servaddr = Address::from_ip(argv[1], SERV_PORT);

    sock.connect(servaddr);

    str_cli(std::cin, sock);

    sock.close();
    return 0;
}
