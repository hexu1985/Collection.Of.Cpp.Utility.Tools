#pragma once

#include <sys/socket.h>
#include <memory>

enum class ByteOrder {
    unknown = 0,
    big_endian = 1,
    little_endian = 2,
};

ByteOrder check_byte_order();

std::shared_ptr<struct addrinfo> Getaddrinfo(const char *host, const char *serv,
        const struct addrinfo *hints); 

void Inet_pton(int family, const char *strptr, void *addrptr); 
std::string Inet_ntop(int family, const void *addrptr); 
std::tuple<std::string, uint16_t> sock_ntop(const struct sockaddr *sa, socklen_t salen); 
