#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    std::cout << "Creating socket... " << std::flush;
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        std::cerr << "socket() error" << std::endl;
        exit(1);
    }

    // 对应 Python 的 s.settimeout(5)
    // 设置发送/接收超时（SO_RCVTIMEO / SO_SNDTIMEO）
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
        std::cerr << "setsockopt(SO_RCVTIMEO) error" << std::endl;
        close(s);
        exit(1);
    }
    if (setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1) {
        std::cerr << "setsockopt(SO_SNDTIMEO) error" << std::endl;
        close(s);
        exit(1);
    }
    std::cout << "done." << std::endl;

    std::cout << "Connecting to remote host... " << std::flush;

    // 通过域名解析获取主机信息
    struct addrinfo hints;
    struct addrinfo* res = nullptr;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("www.google.com", "80", &hints, &res) != 0) {
        std::cerr << "getaddrinfo() error" << std::endl;
        close(s);
        exit(1);
    }

    // 说明：SO_RCVTIMEO/SO_SNDTIMEO 不作用于 connect()。
    // 若要严格实现 Python settimeout(5) 对 connect 的超时，
    // 需在 connect 前把 socket 设为非阻塞，再配合 select/poll 实现超时。
    if (connect(s, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "connect() error" << std::endl;
        freeaddrinfo(res);
        close(s);
        exit(1);
    }

    freeaddrinfo(res);
    std::cout << "done." << std::endl;

    close(s);
    return 0;
}
