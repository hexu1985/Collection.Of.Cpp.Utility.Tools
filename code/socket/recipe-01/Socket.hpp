#pragma once

#include "SocketException.hpp"

#include <sys/types.h>
#include <arpa/inet.h>

#include <tuple>
#include <string>
#include <string_view>

class Socket {
public:
    Socket(int family, int type, int protocol=0);

    Socket(const Socket&) = delete;
    Socket& operator= (const Socket&) = delete;

    Socket(Socket&& other);

    ~Socket();

    void Connect(const char* host, uint16_t port);
    void Connect(const std::tuple<std::string, uint16_t>& address) {
        Connect(std::get<0>(address).c_str(), std::get<1>(address));
    }

    void sendall(std::string_view data);

    void Close();

    void Shutdown(int how);

    void Bind(const char* host, uint16_t port);
    void Bind(const std::tuple<std::string, uint16_t>& address) {
        Bind(std::get<0>(address).c_str(), std::get<1>(address));
    }

    void Listen(int backlog);

    std::tuple<std::string, uint16_t> Getsockname();

    Socket Accept(std::tuple<std::string, uint16_t>* peername=nullptr);

    std::string Recv(size_t len, int flags=0);

    void Setsockopt(int level, int optname, int value);

    size_t Send(const void* buf, size_t len, int flags=0);
    size_t Send(std::string_view data, int flags=0) {
        return Send(data.data(), data.size(), flags);
    }

    std::tuple<std::string, uint16_t> Getpeername();

private:
    Socket() = default;

private:
    int sockfd_=-1;
    int family_=-1;
};


