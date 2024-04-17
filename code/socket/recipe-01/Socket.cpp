#include "Socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#include <memory>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

namespace {

std::shared_ptr<addrinfo> Getaddrinfo(
        const char *host, const char *serv,
        const struct addrinfo *hints) {
    struct addrinfo* res = nullptr;
    int n = 0;
    if ((n = getaddrinfo(host, serv, hints, &res)) != 0) {
        throw GAIError(n, format("Getaddrinfo error for {}, {}",
                    (host == NULL) ? "(no hostname)" : host,
                    (serv == NULL) ? "(no service name)" : serv));
    }

    auto deleter = [](struct addrinfo* res) { if (res) freeaddrinfo(res); };
    return std::shared_ptr<struct addrinfo>(res, deleter);
}

}   // namespace

Socket::Socket(int family, int type, int protocol): family_(family) {
    sockfd_ = socket(family, type, protocol);
    if (sockfd_ < 0) {
        throw SocketError(errno, 
                format("Socket({}, {}, {}) error", family, type, protocol));
    }
}

void Socket::Connect(const char* host, uint16_t port) {
    addrinfo hints{};
    hints.ai_family = family_;

    std::string serv = std::to_string(port);
    auto servinfo = Getaddrinfo(host, serv.c_str(), &hints);

    if (connect(sockfd_, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        throw SocketError(errno, format("Connect({}, {}) error", host, port));
    }
}
