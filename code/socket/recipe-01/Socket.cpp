#include "Socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <cstring>
#include <iostream>

#include <memory>

#if __cplusplus >= 202002L
#include <format>
using std::format;
#else
#define FMT_HEADER_ONLY
#include "fmt/format.h"
using fmt::format;
#endif

std::ostream& operator<< (std::ostream& out, const SocketAddress& address) {
    out << "(" << std::get<0>(address) << ", " << std::get<1>(address) << ")";
    return out;
}

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
        throw OSError(errno, format("inet_pton error for {}", strptr));     /* errno set */
    } else if (n == 0) {
        throw std::runtime_error(format("inet_pton error for {}", strptr));     /* errno not set */
    }

	/* nothing to return */
}

std::string Inet_ntop(int family, const void *addrptr) {
    char buf[INET6_ADDRSTRLEN] = {0};
    const char* ptr;
    if ((ptr = inet_ntop(family, addrptr, buf, sizeof(buf))) == NULL) {
        throw OSError(errno, "Inet_ntop() error");
    }
    return ptr;
}

SocketAddress sock_ntop(const struct sockaddr *sa, socklen_t salen) {
    std::string host;
    uint16_t port;
    switch (sa->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) sa;
        host = Inet_ntop(AF_INET, &sin->sin_addr);
        port = ntohs(sin->sin_port);
        break;
    }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;
        host = Inet_ntop(AF_INET6, &sin6->sin6_addr);
        port = ntohs(sin6->sin6_port); 
        break;
    }

    default:
        std::cerr << format("sock_ntop: unknown AF_xxx: {}, len {}", sa->sa_family, salen) << "\n";
    }
    return SocketAddress{std::move(host), port};
}

}   // namespace

Socket::Socket(int family, int type, int protocol): family_(family) {
    sockfd_ = socket(family, type, protocol);
    if (sockfd_ < 0) {
        throw OSError(errno, 
                format("Socket({}, {}, {}) error", family, type, protocol));
    }
}

Socket::Socket(Socket&& other): sockfd_(other.sockfd_), family_(other.family_) {
    other.sockfd_ = -1;
    other.family_ = -1;
}

Socket::~Socket() {
    if (sockfd_ >= 0) {
        Close();
    }
}

void Socket::Connect(const SocketAddress& address) {
    addrinfo hints{};
    hints.ai_family = family_;

    auto [host, port] = address;
    std::string serv = std::to_string(port);
    auto servinfo = Getaddrinfo(host.c_str(), serv.c_str(), &hints);

    if (connect(sockfd_, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        throw OSError(errno, format("Connect({}, {}) error", host, port));
    }
}

void Socket::sendall(std::string_view buffer) {
    if (writen(sockfd_, buffer.data(), buffer.size()) != buffer.size()) {
        throw OSError(errno, "sendall() error");
    }
}

void Socket::Shutdown(int how) {
    if (shutdown(sockfd_, how) < 0) {
        throw OSError(errno, format("Shutdown({}) error", how));
    }
}

void Socket::Close() {
    if (close(sockfd_) < 0) {
        throw OSError(errno, "Close() error");
    }
    sockfd_ = -1;
}

void Socket::Bind(const SocketAddress& address) {
    const char* host = std::get<0>(address).c_str();
    uint16_t port = std::get<1>(address);

    struct sockaddr_storage storage;
    memset(&storage, 0, sizeof(storage));

    struct sockaddr* sa = nullptr;
    socklen_t salen = 0;
    if (family_ == AF_INET) {
        struct sockaddr_in* sin = reinterpret_cast<struct sockaddr_in*>(&storage);
        sin->sin_family = family_;
        sin->sin_port = htons(port);
        Inet_pton(family_, host, &sin->sin_addr);
        sa = reinterpret_cast<struct sockaddr*>(sin);
        salen = sizeof(struct sockaddr_in);
    } else if (family_ == AF_INET6) {
        struct sockaddr_in6* sin6 = reinterpret_cast<struct sockaddr_in6*>(&storage);
        sin6->sin6_family = family_;
        sin6->sin6_port = htons(port);
        Inet_pton(family_, host, &sin6->sin6_addr);
        sa = reinterpret_cast<struct sockaddr*>(sin6);
        salen = sizeof(struct sockaddr_in6);
    } else {
        throw std::runtime_error(format("Bind({}, {}) error: unsupport family type", host, port));
    }

    if (bind(sockfd_, sa, salen) < 0) {
        throw OSError(errno, format("Bind({}, {})", host, port));
    }
}

void Socket::Listen(int backlog) {
    if (listen(sockfd_, backlog) < 0) {
        throw OSError(errno, format("Listen({}) error", backlog));
    }
}

SocketAddress Socket::Getsockname() {
    struct sockaddr_storage address;
    memset(&address, 0, sizeof(address));

    struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&address);
    socklen_t salen = sizeof(address);

    if (getsockname(sockfd_, sa, &salen) < 0) {
        throw OSError(errno, "Getsockname() error");
    }

    return sock_ntop(sa, salen);
}

Socket Socket::Accept(SocketAddress* address) {
    struct sockaddr_storage storage;
    memset(&storage, 0, sizeof(storage));

    struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&storage);
    socklen_t salen = sizeof(storage);

    int n;
again:
    if ((n = accept(sockfd_, sa, &salen)) < 0) {
        if (errno == EPROTO || errno == ECONNABORTED)
            goto again;
        else
            throw OSError(errno, "Accept() error");
    }

    Socket sock;
    sock.sockfd_ = n;
    sock.family_ = family_;

    if (address) {
        *address = sock_ntop(sa, salen);
    }

    return sock;
}

std::string Socket::Recv(size_t len, int flags) {
    std::unique_ptr<char[]> buf(new char[len]);
    auto n = recv(sockfd_, buf.get(), len, flags);
    if (n < 0) {
        throw OSError(errno, "Recv() error");
    } 
    return std::string(buf.get(), n);
}

void Socket::Setsockopt(int level, int optname, int optval) {
    if (setsockopt(sockfd_, level, optname, &optval, sizeof(optval)) < 0) {
        throw OSError(errno, format("Setsockopt({}, {}, {})", level, optname, optval));
    }
}

size_t Socket::Send(std::string_view buffer, int flags) {
    const void* buf = buffer.data();
    size_t len = buffer.size();

    auto n = send(sockfd_, buf, len, flags);
    if (n < 0) {
        throw OSError(errno, "Send() error");
    }
    return n;
}

SocketAddress Socket::Getpeername() {
    struct sockaddr_storage address;
    memset(&address, 0, sizeof(address));

    struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&address);
    socklen_t salen = sizeof(address);

    if (getpeername(sockfd_, sa, &salen) < 0) {
        throw OSError(errno, "Getpeername() error");
    }

    return sock_ntop(sa, salen);
}

std::string Socket::Recvfrom(size_t len, int flags, SocketAddress* address) {
    struct sockaddr_storage storage;
    memset(&storage, 0, sizeof(storage));

    struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&storage);
    socklen_t salen = sizeof(storage);

    std::unique_ptr<char[]> buf(new char[len]);
    ssize_t n = recvfrom(sockfd_, buf.get(), len, flags, sa, &salen);
    if (n < 0) {
        throw OSError(errno, "Recvfrom() error");
    }

    if (address) {
        *address = sock_ntop(sa, salen);
    }

    return std::string(buf.get(), n);
}

size_t Socket::Sendto(std::string_view buffer, int flags, const SocketAddress& address) {
    addrinfo hints{};
    hints.ai_family = family_;

    auto [host, port] = address;
    std::string serv = std::to_string(port);
    auto servinfo = Getaddrinfo(host.c_str(), serv.c_str(), &hints);

    const void* buf = buffer.data();
    size_t len = buffer.size();
    ssize_t n = sendto(sockfd_, buf, len, flags, servinfo->ai_addr, servinfo->ai_addrlen);
    if (n < 0) {
        throw OSError(errno, format("Sendto({}, {}) error", host, port));
    }
    return n;
}
