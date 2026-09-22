#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define LISTENQ 1024
#define SA struct sockaddr

int main(int argc, char **argv)
{
    int                 listenfd, connfd;
    int                 port;
    struct sockaddr_in  servaddr;
    char                buff[MAXLINE];
    time_t              ticks;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "invalid port: %s\n", argv[1]);
        exit(1);
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket");
        exit(1);
    }

    /* 允许地址重用，避免重启时 "Address already in use" */
    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons((uint16_t) port);   /* port from argv */

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(listenfd, LISTENQ) < 0) {
        perror("listen");
        exit(1);
    }

    for ( ; ; ) {
        connfd = accept(listenfd, (SA *) NULL, NULL);
        if (connfd < 0) {
            perror("accept");
            continue;
        }

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));

        close(connfd);
    }
}
