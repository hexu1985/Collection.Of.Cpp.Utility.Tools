#include "socket.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>

using namespace unpsock;

std::string get_timeday() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm_buf{};                       // 每个线程用自己的缓冲区
    if (localtime_r(&t, &tm_buf) == nullptr) {   // 可重入版本
        std::cerr << "localtime_r failed\n";
        return "\r\n";
    }

    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S\r\n", &tm_buf);
    return buf;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <port>\n";
        exit(1);
    }

    int port = std::stoi(argv[1]);

    Socket listen_sock(Family::INET, AddrType::STREAM);
    listen_sock.setsockopt_as<int>(SOL_SOCKET, SO_REUSEADDR, 1);

    auto servaddr = Address::any(port, Family::INET);
    listen_sock.bind(servaddr);
    listen_sock.listen(5);

    while (true) {
        auto [conn_sock, _] = listen_sock.accept();
        conn_sock.sendall(get_timeday());
        conn_sock.close();
    }

    return 0;
}
