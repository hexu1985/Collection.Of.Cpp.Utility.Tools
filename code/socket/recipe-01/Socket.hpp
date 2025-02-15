#pragma once

#include "SocketException.hpp"

#include <sys/types.h>
#include <arpa/inet.h>

#include <tuple>
#include <string>
#include <string_view>
#include <iosfwd>

using SocketAddress = std::tuple<std::string, int32_t>;

class Socket {
public:
    Socket(int family, int type, int protocol=0);

    Socket(const Socket&) = delete;
    Socket& operator= (const Socket&) = delete;

    Socket(Socket&& other);

    ~Socket();

    void Connect(const SocketAddress& address); 

    void sendall(std::string_view data);

    void Close();

    void Shutdown(int how);

    void Bind(const SocketAddress& address); 

    void Listen(int backlog);

    SocketAddress Getsockname();

    Socket Accept(SocketAddress* address=nullptr);

    std::string Recv(size_t len, int flags=0);

    void Setsockopt(int level, int optname, int value);

    size_t Send(const void* buf, size_t len, int flags=0);
    size_t Send(std::string_view data, int flags=0) {
        return Send(data.data(), data.size(), flags);
    }

    SocketAddress Getpeername();

private:
    Socket() = default;

private:
    int sockfd_=-1;
    int family_=-1;
};

std::ostream& operator<< (std::ostream& out, const SocketAddress& address);

