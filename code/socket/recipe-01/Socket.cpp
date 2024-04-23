#include "Socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
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

ssize_t                     /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
    size_t      nleft;
    ssize_t     nwritten;
    const char  *ptr;

    ptr = (const char *) vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;       /* and call write() again */
            else
                return(-1);         /* error */
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
/* end writen */

void Inet_pton(int family, const char *strptr, void *addrptr) {
    int		n;

	if ( (n = inet_pton(family, strptr, addrptr)) < 0) {
        throw SocketError(errno, format("inet_pton error for {}", strptr));     /* errno set */
    } else if (n == 0) {
        throw std::runtime_error(format("inet_pton error for {}", strptr));     /* errno not set */
    }

	/* nothing to return */
}

std::string Inet_ntop(int family, const void *addrptr) {
    char buf[INET6_ADDRSTRLEN] = {0};
    const char* ptr;
    if ((ptr = inet_ntop(family, addrptr, buf, sizeof(buf))) == NULL) {
        throw SocketError(errno, "Inet_ntop() error");
    }
    return ptr;
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

void Socket::Sendall(std::string_view data) {
    if (writen(sockfd_, data.data(), data.size()) != data.size()) {
        throw SocketError(errno, "Sendall() error");
    }
}

void Socket::Shutdown(int how) {
    if (shutdown(sockfd_, how) < 0) {
        throw SocketError(errno, format("Shutdown({}) error", how));
    }
}

void Socket::Close() {
    if (close(sockfd_) < 0) {
        throw SocketError(errno, "Close() error");
    }
}

void Socket::Bind(const char* host, uint16_t port) {
    struct sockaddr_storage address;
    memset(&address, 0, sizeof(address));

    struct sockaddr* sa = nullptr;
    socklen_t salen = 0;
    if (family_ == AF_INET) {
        struct sockaddr_in* sin = reinterpret_cast<struct sockaddr_in*>(&address);
        sin->sin_family = family_;
        sin->sin_port = htons(port);
        Inet_pton(family_, host, &sin->sin_addr);
        sa = reinterpret_cast<struct sockaddr*>(sin);
        salen = sizeof(struct sockaddr_in);
    } else {
        throw std::runtime_error(format("Bind({}, {}) error: unsupport family type", host, port));
    }

    if (bind(sockfd_, sa, salen) < 0) {
        throw SocketError(errno, format("Bind({}, {})", host, port));
    }
}

void Socket::Listen(int backlog) {
    if (listen(sockfd_, backlog) < 0) {
        throw SocketError(errno, format("Listen({}) error", backlog));
    }
}

std::tuple<std::string, uint16_t> Socket::Getsockname() {
    struct sockaddr_storage address;
    memset(&address, 0, sizeof(address));

    struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&address);
    socklen_t salen = sizeof(address);

    if (getsockname(sockfd_, sa, &salen) < 0) {
        throw SocketError(errno, "Getsockname() error");
    }

    std::string host;
    uint16_t port;
    if (family_ == AF_INET) {
        struct sockaddr_in* sin = reinterpret_cast<struct sockaddr_in*>(&address);
        host = Inet_ntop(family_, &sin->sin_addr);
        port = ntohs(sin->sin_port);
    } else {
        throw std::runtime_error("Getsockname() error: unsupport family type");
    }

    return std::make_tuple(std::move(host), port);
}
