#pragma once

#include <sys/socket.h>

class SocketAddress {
public:
    SocketAddress();
    SocketAddress(struct sockaddr* addr, socklen_t addr_len);

    bool is_empty() const {
        return addr_len_ == 0;
    }

    struct sockaddr* addr_ptr() {
        return reinterpret_cast<struct sockaddr*>(&addr_);
    }

    const struct sockaddr* addr_ptr() const {
        return reinterpret_cast<const struct sockaddr*>(&addr_);
    }

    socklen_t* addr_len_ptr() {
        return &addr_len_;
    }

    const socklen_t* addr_len_ptr() const {
        return &addr_len_;
    }

private:
    struct sockaddr_storage addr_;
    socklen_t addr_len_ = 0;
};
