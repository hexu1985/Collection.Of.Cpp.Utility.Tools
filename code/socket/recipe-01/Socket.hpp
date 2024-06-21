#pragma once

#include "SocketException.hpp"

#include <sys/types.h>
#include <arpa/inet.h>

#include <tuple>
#include <string>

class Socket {
public:
    Socket(int family, int type, int protocol=0);

    Socket(const Socket&) = delete;
    Socket& operator= (const Socket&) = delete;

    Socket(Socket&& other);

    ~Socket();

    void Connect(const char* host, uint16_t port);

    void Sendall(std::string_view data);

    void Close();

    void Shutdown(int how);

    void Bind(const char* host, uint16_t port);

    void Listen(int backlog);

    std::tuple<std::string, uint16_t> Getsockname();

    Socket Accept(std::tuple<std::string, uint16_t>* peername=nullptr);

    std::string Recv(size_t len);

    void Setsockopt(int level, int optname, int value);

    size_t Send(std::string_view data, int flags=0);

    std::tuple<std::string, uint16_t> Getpeername();

private:
    Socket() = default;

private:
    int sockfd_=-1;
    int family_=-1;
};


