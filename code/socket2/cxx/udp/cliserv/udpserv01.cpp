#include "socket.hpp"

#include <iostream>

using namespace unpsock;

const int SERV_PORT = 9877;   // 对应 C 代码中的 SERV_PORT
const int MAXLINE = 4096;     // 对应 MAXLINE

// 对应 C 中的 dg_echo：把收到的数据报原样回显给发送方
void dg_echo(Socket& sock)
{
    for ( ; ; ) {
        auto [mesg, cliaddr] = sock.recvfrom(MAXLINE);
        sock.sendto(mesg, cliaddr);
    }
}

int main()
{
    Socket sock(Family::INET, AddrType::DGRAM);

    sock.bind(Address::any(SERV_PORT));

    dg_echo(sock);

    // 不会执行到这里
    sock.close();

    return 0;
}
