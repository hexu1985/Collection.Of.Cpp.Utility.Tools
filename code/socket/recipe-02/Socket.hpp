#pragma once

#include "SocketException.hpp"
#include "SocketAddress.hpp"

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
    void Connect(const std::tuple<std::string, uint16_t>& address) {
        Connect(std::get<0>(address).c_str(), std::get<1>(address));
    }

    void sendall(StringView data);

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
    size_t Send(StringView data, int flags=0) {
        return Send(data.data(), data.size(), flags);
    }

    std::tuple<std::string, uint16_t> Getpeername();

    std::string Recvfrom(size_t len, int flags, SocketAddress& src_addr);
    std::string Recvfrom(size_t len, SocketAddress& src_addr) {
        return Recvfrom(len, 0, src_addr);
    }

    size_t Sendto(const void* buf, size_t len, int flags, const SocketAddress& dst_addr);
    size_t Sendto(const void* buf, size_t len, const SocketAddress& dst_addr) {
        return Sendto(buf, len, 0, dst_addr);
    }
    size_t Sendto(StringView data, int flags, const SocketAddress& dst_addr) {
        return Sendto(data.data(), data.size(), flags, dst_addr);
    }
    size_t Sendto(StringView data, const SocketAddress& dst_addr) {
        return Sendto(data.data(), data.size(), 0, dst_addr);
    }

    size_t Sendto(const void* buf, size_t len, int flags, const char* host, uint16_t port); 
    size_t Sendto(const void* buf, size_t len, const char* host, uint16_t port) {
        return Sendto(buf, len, 0, host, port);
    }
    size_t Sendto(StringView data, int flags, const char* host, uint16_t port) {
        return Sendto(data.data(), data.size(), flags, host, port);
    }
    size_t Sendto(StringView data, const char* host, uint16_t port) {
        return Sendto(data.data(), data.size(), 0, host, port);
    }

    size_t Sendto(const void* buf, size_t len, int flags, const std::tuple<std::string, uint16_t>& address) {
        return Sendto(buf, len, flags, std::get<0>(address).c_str(), std::get<1>(address));
    }
    size_t Sendto(const void* buf, size_t len, const std::tuple<std::string, uint16_t>& address) {
        return Sendto(buf, len, 0, address);
    }
    size_t Sendto(StringView data, int flags, const std::tuple<std::string, uint16_t>& address) {
        return Sendto(data.data(), data.size(), flags, address);
    }
    size_t Sendto(StringView data, const std::tuple<std::string, uint16_t>& address) {
        return Sendto(data.data(), data.size(), 0, address);
    }

private:
    Socket() = default;

private:
    int sockfd_=-1;
    int family_=-1;
};

