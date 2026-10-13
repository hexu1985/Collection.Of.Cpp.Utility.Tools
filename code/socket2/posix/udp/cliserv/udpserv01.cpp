#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int SERV_PORT = 9877;   // 对应 C 代码中的 SERV_PORT
const int MAXLINE = 4096;     // 对应 MAXLINE

// 对应 C 中的 dg_echo：把收到的数据报原样回显给发送方
void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int n;
    socklen_t len;
    char mesg[MAXLINE];

    for (;;) {
        len = clilen;
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        if (n < 0) {
            if (errno == EINTR)
                continue;   // 被信号打断，重试
            perror("recvfrom");
            std::exit(1);
        }

        // 把收到的数据报原样发回
        if (sendto(sockfd, mesg, n, 0, pcliaddr, len) < 0) {
            if (errno == EINTR)
                continue;
            perror("sendto");
            std::exit(1);
        }
    }
}

int main()
{
    // 创建 UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        std::exit(1);
    }

    // 绑定到 INADDR_ANY:SERV_PORT
    // 等价于 sin_addr.s_addr = htonl(INADDR_ANY), sin_port = htons(SERV_PORT)
    struct sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(sockfd, reinterpret_cast<struct sockaddr *>(&servaddr),
             sizeof(servaddr)) < 0) {
        perror("bind");
        std::exit(1);
    }

    // 客户端地址缓冲区，dg_echo 会填充它
    struct sockaddr_in cliaddr;

    dg_echo(sockfd,
            reinterpret_cast<struct sockaddr *>(&cliaddr),
            sizeof(cliaddr));

    // 不会执行到这里
    close(sockfd);
    return 0;
}
