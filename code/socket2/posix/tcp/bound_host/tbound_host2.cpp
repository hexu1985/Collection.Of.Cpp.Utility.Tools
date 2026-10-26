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
    if (argc != 3) {
        std::cout << "Usage : " << argv[0] << " <IP> <port>" << std::endl;
        exit(1);
    }

    const char* host = argv[1];
    int port = std::atoi(argv[2]);

    const char* msg1 = "Hi!";
    const char* msg2 = "I'm another UDP host!";
    const char* msg3 = "Nice to meet you";

    // socket(PF_INET, SOCK_STREAM, 0)
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in serv_adr;
    std::memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(host);
    serv_adr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("connect() error");
    }

    write(sock, msg1, std::strlen(msg1));
    write(sock, msg2, std::strlen(msg2));
    write(sock, msg3, std::strlen(msg3));

    close(sock);

    return 0;
}
