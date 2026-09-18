#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void error_handling(const char *buf);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];

    struct pollfd poll_fds[MAX_CLNT];
    int nfds = 0;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // 监听 socket 放在数组第一个位置
    poll_fds[0].fd = serv_sock;
    poll_fds[0].events = POLLIN;
    nfds = 1;

    while (1)
    {
        int timeout_ms = 5000;  // 5 秒
        int ret = poll(poll_fds, nfds, timeout_ms);

        if (ret == -1)
            break;
        if (ret == 0)
            continue;  // 超时

        for (i = 0; i < nfds; i++)
        {
            if (poll_fds[i].revents & POLLIN)
            {
                if (poll_fds[i].fd == serv_sock)  // 新连接
                {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    if (clnt_sock == -1)
                        continue;

                    if (nfds < MAX_CLNT) {
                        poll_fds[nfds].fd = clnt_sock;
                        poll_fds[nfds].events = POLLIN;
                        nfds++;
                        printf("connected client: %d \n", clnt_sock);
                    } else {
                        close(clnt_sock);
                    }
                }
                else  // 客户端发来数据
                {
                    str_len = read(poll_fds[i].fd, buf, BUF_SIZE);
                    if (str_len == 0)  // 客户端关闭
                    {
                        printf("closed client: %d \n", poll_fds[i].fd);
                        close(poll_fds[i].fd);
                        // 用最后一个元素覆盖当前位置，然后 nfds--
                        poll_fds[i] = poll_fds[nfds - 1];
                        nfds--;
                        i--;  // 重新检查被搬过来的这个位置
                    }
                    else
                    {
                        write(poll_fds[i].fd, buf, str_len);  // echo
                    }
                }
            }
        }
    }

    close(serv_sock);
    return 0;
}

void error_handling(const char *buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}
