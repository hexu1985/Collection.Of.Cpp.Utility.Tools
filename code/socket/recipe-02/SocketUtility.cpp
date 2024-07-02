#include "SocketUtility.hpp"
#include "SocketException.hpp"

#include <netdb.h>
#include <arpa/inet.h>

#include <stdint.h>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

using fmt::format;

ByteOrder check_byte_order() {
    union {
        uint16_t s;
        char     c[sizeof(uint16_t)];
    } un;

    un.s = 0x0102;
    if (un.c[0] == 1 && un.c[1] == 2)
        return ByteOrder::big_endian;
    else if (un.c[0] == 2 && un.c[1] == 1)
        return ByteOrder::little_endian;
    else
        return ByteOrder::unknown;
}

std::shared_ptr<struct addrinfo> Getaddrinfo(
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

std::tuple<std::string, uint16_t> sock_ntop(const struct sockaddr *sa, socklen_t salen) {
    std::string host;
    uint16_t port=0;
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
        throw std::runtime_error(format("sock_ntop: unknown AF_xxx: {}, len {}", sa->sa_family, salen));
    }
    return std::make_tuple(std::move(host), port);
}

