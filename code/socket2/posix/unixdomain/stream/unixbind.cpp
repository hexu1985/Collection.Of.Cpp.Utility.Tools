#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: unixbind <pathname>" << std::endl;
        return 1;
    }

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // 如果文件已存在则删除（失败也无所谓）
    unlink(argv[1]);

    struct sockaddr_un addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, argv[1], sizeof(addr.sun_path) - 1);

    if (bind(sockfd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    // 获取绑定后的地址
    struct sockaddr_un addr2;
    socklen_t len = sizeof(addr2);
    std::memset(&addr2, 0, sizeof(addr2));

    if (getsockname(sockfd, reinterpret_cast<struct sockaddr *>(&addr2), &len) < 0) {
        perror("getsockname");
        close(sockfd);
        return 1;
    }

    // 打印路径和长度（与 Python 输出格式保持一致）
    std::cout << "bound name = " << addr2.sun_path
              << ", returned len = " << (std::strlen(addr2.sun_path) + 1)
              << std::endl;

    close(sockfd);
    return 0;
}
