#include "Socket.hpp"
#include "SocketUtility.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <iostream>

#include <memory>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

namespace {

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

}   // namespace

Socket::Socket(int family, int type, int protocol): family_(family) {
    sockfd_ = socket(family, type, protocol);
    if (sockfd_ < 0) {
        throw SocketError(errno, 
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

void Socket::Connect(const char* host, uint16_t port) {
    addrinfo hints{};
    hints.ai_family = family_;

    std::string serv = std::to_string(port);
    auto servinfo = Getaddrinfo(host, serv.c_str(), &hints);

    if (connect(sockfd_, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        throw SocketError(errno, format("Connect({}, {}) error", host, port));
    }
}

void Socket::sendall(std::string_view data) {
    if (writen(sockfd_, data.data(), data.size()) != data.size()) {
        throw SocketError(errno, "sendall() error");
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
    sockfd_ = -1;
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
    } else if (family_ == AF_INET6) {
        struct sockaddr_in6* sin6 = reinterpret_cast<struct sockaddr_in6*>(&address);
        sin6->sin6_family = family_;
        sin6->sin6_port = htons(port);
        Inet_pton(family_, host, &sin6->sin6_addr);
        sa = reinterpret_cast<struct sockaddr*>(sin6);
        salen = sizeof(struct sockaddr_in6);
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

    return sock_ntop(sa, salen);
}

Socket Socket::Accept(std::tuple<std::string, uint16_t>* peername) {
    struct sockaddr_storage address;
    memset(&address, 0, sizeof(address));

    struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&address);
    socklen_t salen = sizeof(address);

    int n;
again:
    if ((n = accept(sockfd_, sa, &salen)) < 0) {
        if (errno == EPROTO || errno == ECONNABORTED)
            goto again;
        else
            throw SocketError(errno, "Accept() error");
    }

    Socket sock;
    sock.sockfd_ = n;
    sock.family_ = family_;

    if (peername) {
        *peername = sock_ntop(sa, salen);
    }

    return sock;
}

std::string Socket::Recv(size_t len, int flags) {
    std::unique_ptr<char[]> buf(new char[len]);
    auto n = recv(sockfd_, buf.get(), len, flags);
    if (n < 0) {
        throw SocketError(errno, "Recv() error");
    } 
    return std::string(buf.get(), n);
}

void Socket::Setsockopt(int level, int optname, int optval) {
    if (setsockopt(sockfd_, level, optname, &optval, sizeof(optval)) < 0) {
        throw SocketError(errno, format("Setsockopt({}, {}, {})", level, optname, optval));
    }
}

size_t Socket::Send(const void* buf, size_t len, int flags) {
    auto n = send(sockfd_, buf, len, flags);
    if (n < 0) {
        throw SocketError(errno, "Send() error");
    }
    return n;
}

std::tuple<std::string, uint16_t> Socket::Getpeername() {
    struct sockaddr_storage address;
    memset(&address, 0, sizeof(address));

    struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&address);
    socklen_t salen = sizeof(address);

    if (getpeername(sockfd_, sa, &salen) < 0) {
        throw SocketError(errno, "Getpeername() error");
    }

    return sock_ntop(sa, salen);
}

std::string Socket::Recvfrom(size_t len, int flags, SocketAddress& src_addr) {
    src_addr.clean();
    struct sockaddr* addr = src_addr.addr_ptr(); 
    socklen_t* addrlen = src_addr.addr_len_ptr();

    std::unique_ptr<char[]> buf(new char[len]);
    ssize_t n = recvfrom(sockfd_, buf.get(), len, flags, addr, addrlen);
    if (n < 0) {
        throw SocketError(errno, "Recvfrom() error");
    }
    return std::string(buf.get(), n);
}

size_t Socket::Sendto(const void* buf, size_t len, int flags, const SocketAddress& dest_addr) {
    const struct sockaddr* addr = dest_addr.addr_ptr(); 
    socklen_t addrlen = *dest_addr.addr_len_ptr(); 

    ssize_t n = sendto(sockfd_, buf, len, flags, addr, addrlen);
    if (n < 0) {
        throw SocketError(errno, format("Sendto({}) error", dest_addr.to_string()));
    }
    return n;
}

size_t Socket::Sendto(const void* buf, size_t len, int flags, const char* host, uint16_t port) {
    addrinfo hints{};
    hints.ai_family = family_;

    std::string serv = std::to_string(port);
    auto servinfo = Getaddrinfo(host, serv.c_str(), &hints);

    ssize_t n = sendto(sockfd_, buf, len, flags, servinfo->ai_addr, servinfo->ai_addrlen);
    if (n < 0) {
        throw SocketError(errno, format("Sendto({}, {}) error", host, port));
    }
    return n;
}
