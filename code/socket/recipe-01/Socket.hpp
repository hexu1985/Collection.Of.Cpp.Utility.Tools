#pragma once

#include "SocketError.hpp"

#include <sys/types.h>
#include <arpa/inet.h>

class Socket {
public:
    Socket(int family, int type, int protocol=0);

    void Connect(const char* host, uint16_t port);

    void Sendall(std::string_view data);

    void Close();

    void Shutdown(int);

private:
    int sockfd_;
    int family_;
};


