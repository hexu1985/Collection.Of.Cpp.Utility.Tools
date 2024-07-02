#pragma once

#include <sys/socket.h>

class SocketAddress {
public:
    SocketAddress(struct sockaddr* addr, socklen_t addr_len);

    struct sockaddr* addr_ptr() {
        return reinterpret_cast<struct sockaddr*>(&addr_);
    }

    socklen_t* addr_len_ptr() {
        return &addr_len_;
    }

private:
    struct sockaddr_storage addr_;
    socklen_t addr_len_ = 0;
};
