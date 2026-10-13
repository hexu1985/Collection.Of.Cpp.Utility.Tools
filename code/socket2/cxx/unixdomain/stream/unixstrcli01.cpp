#include "socket.hpp"

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace unpsock;

const char *UNIXSTR_PATH = "/tmp/unixstr";  // 对应 C 代码中的 UNIXSTR_PATH
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
    Socket sock(Family::UNIX, AddrType::STREAM);

    sock.connect(Address::from_unix(UNIXSTR_PATH));

    str_cli(std::cin, sock);

    sock.close();
    return 0;
}
