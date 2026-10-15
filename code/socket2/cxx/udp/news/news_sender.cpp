#include "socket.hpp"

#include <netinet/in.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace unpsock;

static constexpr int   TTL      = 64;
static constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage : " << argv[0] << " <GroupIP> <PORT>\n";
        return 1;
    }

    try {
        const std::string group_ip = argv[1];
        const uint16_t port = static_cast<uint16_t>(std::stoi(argv[2]));

        // 1. UDP socket
        Socket send_sock(Family::INET, AddrType::DGRAM);

        // 2. 组播 TTL（注意类型是 unsigned char）
        send_sock.setsockopt<unsigned char>(
            IPPROTO_IP, IP_MULTICAST_TTL,
            static_cast<unsigned char>(TTL));

        // 3. 目标地址
        Address mul_addr = Address::from_ip(group_ip, port, Family::INET);

        // 4. 打开 news.txt
        std::ifstream fp("news.txt", std::ios::binary);
        if (!fp) {
            std::cerr << "fopen() error\n";
            return 1;
        }

        // 5. 循环发送
        std::vector<char> buf(BUF_SIZE);
        while (fp.getline(buf.data(), static_cast<std::streamsize>(BUF_SIZE))) {
            // 注意：getline 丢弃了 '\n'，原 Python 保留 '\n'
            // 如果要严格对齐 Python，用下面的 readline 版本
            size_t n = static_cast<size_t>(fp.gcount());
            send_sock.sendto(buf.data(), n, mul_addr);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // 6. 析构自动 close
    } catch (const SocketException& e) {
        std::cerr << "socket error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
