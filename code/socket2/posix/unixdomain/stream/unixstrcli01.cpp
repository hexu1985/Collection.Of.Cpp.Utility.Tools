#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

const char *UNIXSTR_PATH = "/tmp/unixstr";  // 对应 C 代码中的 UNIXSTR_PATH
const int MAXLINE = 4096;

// 保证把 n 字节全部写出去
ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft = n;
    const char *ptr = static_cast<const char *>(vptr);
    ssize_t nwritten;

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;  // 被信号打断，重试
            else
                return -1;      // 出错
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

// 从 socket 逐字节读取直到遇到 '\n' 或 EOF
// 返回读取的字节数（0 表示 EOF）
ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    size_t n;
    ssize_t rc;
    char c, *ptr = static_cast<char *>(vptr);

    for (n = 1; n < maxlen; n++) {
    again:
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;  // 遇到换行，结束
        } else if (rc == 0) {
            *ptr = 0;
            return n - 1;  // EOF
        } else {
            if (errno == EINTR)
                goto again;  // 被信号打断，重试
            return -1;       // 出错
        }
    }
    *ptr = 0;
    return n;
}

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE];
    char recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != nullptr) {
        // 发送整行
        if (writen(sockfd, sendline, std::strlen(sendline)) < 0) {
            std::cerr << "writen error" << std::endl;
            std::exit(1);
        }

        // 读取一行
        ssize_t n = readline(sockfd, recvline, MAXLINE);
        if (n < 0) {
            std::cerr << "readline error" << std::endl;
            std::exit(1);
        } else if (n == 0) {
            std::cerr << "str_cli: server terminated prematurely" << std::endl;
            std::exit(1);
        }

        // 写到标准输出
        fputs(recvline, stdout);
        fflush(stdout);
    }
}

int main()
{
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        std::exit(1);
    }

    struct sockaddr_un servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    std::strncpy(servaddr.sun_path, UNIXSTR_PATH, sizeof(servaddr.sun_path) - 1);

    if (connect(sockfd, reinterpret_cast<struct sockaddr *>(&servaddr),
                sizeof(servaddr)) < 0) {
        std::cerr << "connect error: " << std::strerror(errno) << std::endl;
        std::exit(1);
    }

    str_cli(stdin, sockfd);

    close(sockfd);
    return 0;
}
