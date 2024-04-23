#pragma once

#include "SocketException.hpp"

#include <sys/types.h>
#include <arpa/inet.h>

#include <tuple>
#include <string>

class Socket {
public:
    Socket(int family, int type, int protocol=0);

    void Connect(const char* host, uint16_t port);

    void Sendall(std::string_view data);

    void Close();

    void Shutdown(int how);

    void Bind(const char* host, uint16_t port);

    void Listen(int backlog);

    std::tuple<std::string, uint16_t> Getsockname();

private:
    int sockfd_;
    int family_;
};


