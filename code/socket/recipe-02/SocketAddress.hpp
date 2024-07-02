#pragma once

#include <sys/socket.h>
#include <string>

class SocketAddress {
public:
    SocketAddress();
    SocketAddress(struct sockaddr* addr, socklen_t addr_len);

    void clean();

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

    std::string to_string() const;

private:
    struct sockaddr_storage addr_;
    socklen_t addr_len_ = 0;
};
