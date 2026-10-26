#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUF_SIZE 30

void error_handling(const std::string& message) {
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage : " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    int port = std::atoi(argv[1]);

    // 创建TCP套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    // 开始监听，最大等待队列为5
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1) {
        error_handling("accept() error");
    }
    std::cout << "Connected client " << std::endl;

    char message[BUF_SIZE];
    int i = 0;
    while (true) {
        sleep(5);  // sleep(5)：delay 5 sec

        int str_len = read(clnt_sock, message, BUF_SIZE-1);
        if (str_len <= 0) {  // 收到空数据表示客户端关闭了连接
            break;
        }
        message[str_len] = '\0';
        std::cout << "Message " << (i + 1) << ": " << message << " " << std::endl;
        i++;
    }

    close(clnt_sock);
    close(serv_sock);

    return 0;
}
