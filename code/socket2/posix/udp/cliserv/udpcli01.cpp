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

// 对应 C 中的 dg_cli：从 fp 读一行发给服务器，收一个数据报写到 stdout
void dg_cli(FILE *fp, int sockfd, const struct sockaddr *servaddr,
            socklen_t servlen)
{
    char sendline[MAXLINE];
    char recvline[MAXLINE + 1];

    while (fgets(sendline, MAXLINE, fp) != nullptr) {
        // 发送数据报
        ssize_t n = sendto(sockfd, sendline, std::strlen(sendline), 0,
                           servaddr, servlen);
        if (n < 0) {
            perror("sendto");
            std::exit(1);
        }

        // 接收数据报
        n = recvfrom(sockfd, recvline, MAXLINE, 0, nullptr, nullptr);
        if (n < 0) {
            perror("recvfrom");
            std::exit(1);
        }

        recvline[n] = 0;   // 确保字符串以 '\0' 结尾
        fputs(recvline, stdout);
        fflush(stdout);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: udpcli <IPaddress>" << std::endl;
        return 1;
    }

    // 服务器地址（对应 bzero + sin_family + sin_port + Inet_pton）
    struct sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        std::cerr << "inet_pton error for " << argv[1] << std::endl;
        std::exit(1);
    }

    // 创建 UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        std::exit(1);
    }

    dg_cli(stdin, sockfd,
           reinterpret_cast<struct sockaddr *>(&servaddr),
           sizeof(servaddr));

    close(sockfd);
    return 0;
}
