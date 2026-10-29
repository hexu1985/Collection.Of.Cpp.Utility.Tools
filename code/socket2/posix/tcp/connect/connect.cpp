#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
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
    std::cout << "done." << std::endl;

    std::cout << "Connecting to remote host... " << std::flush;

    // 通过域名解析获取主机信息（等价于 Python 的 connect 内部自动解析）
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
