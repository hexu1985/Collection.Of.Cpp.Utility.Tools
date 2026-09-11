// socket_address.hpp
#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <stdexcept>
#include <optional>
#include <cerrno>
#include <cstddef>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

namespace Socket {

// ---------- 地址族 ----------
enum class Family {
    INET,
    INET6,
    UNIX,
    UNSPEC,
};

// ---------- 地址类型 ----------
enum class AddrType {
    STREAM,
    DGRAM,
};

// ---------- 异常 ----------
class AddressError : public std::runtime_error {
public:
    explicit AddressError(const std::string& msg, int errnum = 0)
        : std::runtime_error(msg), errnum_(errnum) {}

    int errnum() const noexcept { return errnum_; }

private:
    int errnum_;
};

// ---------- 内部辅助（inline） ----------
namespace detail {

inline int to_af(Family f) {
    switch (f) {
        case Family::INET:  return AF_INET;
        case Family::INET6: return AF_INET6;
        case Family::UNIX:  return AF_UNIX;
        default:            return AF_UNSPEC;
    }
}

inline Family from_af(int af) {
    switch (af) {
        case AF_INET:  return Family::INET;
        case AF_INET6: return Family::INET6;
        case AF_UNIX:  return Family::UNIX;
        default:       return Family::UNSPEC;
    }
}

inline int to_socktype(AddrType t) {
    return t == AddrType::STREAM ? SOCK_STREAM : SOCK_DGRAM;
}

} // namespace detail

// ---------- 地址类 ----------
class Address {
public:
    Address() = default;

    Address(const Address& other)
        : len_(other.len_), family_(other.family_) {
        std::memcpy(&storage_, &other.storage_, sizeof(storage_));
    }

    Address& operator=(const Address& other) {
        if (this != &other) {
            storage_ = other.storage_;
            len_ = other.len_;
            family_ = other.family_;
        }
        return *this;
    }

    ~Address() = default;

    // ===== 从字符串构造（不做 DNS）=====
    static Address from_ip(const std::string& ip, uint16_t port,
                           Family family = Family::UNSPEC) {
        Address addr;

        if (family == Family::UNSPEC || family == Family::INET) {
            auto* sin = reinterpret_cast<struct sockaddr_in*>(&addr.storage_);
            sin->sin_family = AF_INET;
            sin->sin_port = htons(port);
            if (::inet_pton(AF_INET, ip.c_str(), &sin->sin_addr) == 1) {
                addr.len_ = sizeof(struct sockaddr_in);
                addr.family_ = Family::INET;
                return addr;
            }
        }

        if (family == Family::UNSPEC || family == Family::INET6) {
            auto* sin6 = reinterpret_cast<struct sockaddr_in6*>(&addr.storage_);
            sin6->sin6_family = AF_INET6;
            sin6->sin6_port = htons(port);
            if (::inet_pton(AF_INET6, ip.c_str(), &sin6->sin6_addr) == 1) {
                addr.len_ = sizeof(struct sockaddr_in6);
                addr.family_ = Family::INET6;
                return addr;
            }
        }

        return addr;
    }

    // ===== Unix 域 =====
    static Address from_unix(const std::string& path) {
        Address addr;
        if (path.size() >= sizeof(((struct sockaddr_un*)nullptr)->sun_path)) {
            return addr;
        }

        auto* sun = reinterpret_cast<struct sockaddr_un*>(&addr.storage_);
        sun->sun_family = AF_UNIX;
        std::strncpy(sun->sun_path, path.c_str(), sizeof(sun->sun_path) - 1);
        sun->sun_path[sizeof(sun->sun_path) - 1] = '\0';

        addr.len_ = static_cast<socklen_t>(
            offsetof(struct sockaddr_un, sun_path) + path.size() + 1);
        addr.family_ = Family::UNIX;
        return addr;
    }

    // ===== DNS 解析 =====
    static std::vector<Address> resolve_all(const std::string& host,
                                            uint16_t port,
                                            Family family = Family::UNSPEC,
                                            AddrType type = AddrType::STREAM) {
        struct addrinfo hints{};
        hints.ai_family = detail::to_af(family);
        hints.ai_socktype = detail::to_socktype(type);
        hints.ai_flags = AI_CANONNAME;

        const char* node = host.empty() ? nullptr : host.c_str();
        std::string port_str = std::to_string(port);

        struct addrinfo* result = nullptr;
        int rc = ::getaddrinfo(node, port_str.c_str(), &hints, &result);
        if (rc != 0) {
            throw AddressError(std::string("getaddrinfo: ") + gai_strerror(rc), rc);
        }

        std::vector<Address> out;
        for (auto* p = result; p != nullptr; p = p->ai_next) {
            out.push_back(Address::from_sockaddr(p->ai_addr, p->ai_addrlen));
        }
        ::freeaddrinfo(result);

        return out;
    }

    static Address resolve(const std::string& host, uint16_t port,
                           Family family = Family::UNSPEC,
                           AddrType type = AddrType::STREAM) {
        auto all = resolve_all(host, port, family, type);
        return all.front();
    }

    // ===== 从 sockaddr 构造 =====
    static Address from_sockaddr(const struct sockaddr* sa, socklen_t len) {
        Address addr;
        if (sa == nullptr || len == 0) {
            return addr;
        }

        if (len > sizeof(addr.storage_)) {
            return addr;
        }

        std::memcpy(&addr.storage_, sa, len);
        addr.len_ = len;
        addr.family_ = detail::from_af(sa->sa_family);
        return addr;
    }

    // ===== 转字符串 =====
    std::string ip() const {
        char buf[INET6_ADDRSTRLEN] = {0};

        switch (family_) {
            case Family::INET: {
                auto* sin = reinterpret_cast<const struct sockaddr_in*>(&storage_);
                if (::inet_ntop(AF_INET, &sin->sin_addr, buf, sizeof(buf)) == nullptr) {
                    return "";
                }
                return buf;
            }
            case Family::INET6: {
                auto* sin6 = reinterpret_cast<const struct sockaddr_in6*>(&storage_);
                if (::inet_ntop(AF_INET6, &sin6->sin6_addr, buf, sizeof(buf)) == nullptr) {
                    return "";
                }
                return buf;
            }
            case Family::UNIX: {
                auto* sun = reinterpret_cast<const struct sockaddr_un*>(&storage_);
                return sun->sun_path;
            }
            default:
                return "";
        }
    }

    std::string to_string() const {
        if (family_ == Family::UNIX) {
            return ip();
        }
        if (family_ == Family::INET6) {
            return "[" + ip() + "]:" + std::to_string(port());
        }
        return ip() + ":" + std::to_string(port());
    }

    // ===== 属性 =====
    Family family() const noexcept { return family_; }

    uint16_t port() const noexcept {
        if (family_ == Family::INET) {
            auto* sin = reinterpret_cast<const struct sockaddr_in*>(&storage_);
            return ntohs(sin->sin_port);
        }
        if (family_ == Family::INET6) {
            auto* sin6 = reinterpret_cast<const struct sockaddr_in6*>(&storage_);
            return ntohs(sin6->sin6_port);
        }
        return 0;
    }

    bool is_ipv4() const noexcept { return family_ == Family::INET; }
    bool is_ipv6() const noexcept { return family_ == Family::INET6; }
    bool is_unix() const noexcept { return family_ == Family::UNIX; }
    bool empty() const noexcept { return family_ == Family::UNSPEC; }

    // ===== 底层访问 =====
    const struct sockaddr* sockaddr_ptr() const noexcept {
        return reinterpret_cast<const struct sockaddr*>(&storage_);
    }

    struct sockaddr* sockaddr_ptr() noexcept {
        return reinterpret_cast<struct sockaddr*>(&storage_);
    }

    socklen_t sockaddr_len() const noexcept { return len_; }

    struct sockaddr_storage to_storage() const { return storage_; }

    // ===== 比较 =====
    bool operator==(const Address& other) const noexcept {
        if (family_ != other.family_) return false;

        switch (family_) {
            case Family::INET: {
                auto* a = reinterpret_cast<const struct sockaddr_in*>(&storage_);
                auto* b = reinterpret_cast<const struct sockaddr_in*>(&other.storage_);
                return a->sin_port == b->sin_port &&
                       a->sin_addr.s_addr == b->sin_addr.s_addr;
            }
            case Family::INET6: {
                auto* a = reinterpret_cast<const struct sockaddr_in6*>(&storage_);
                auto* b = reinterpret_cast<const struct sockaddr_in6*>(&other.storage_);
                return a->sin6_port == b->sin6_port &&
                       std::memcmp(&a->sin6_addr, &b->sin6_addr, 16) == 0;
            }
            case Family::UNIX: {
                auto* a = reinterpret_cast<const struct sockaddr_un*>(&storage_);
                auto* b = reinterpret_cast<const struct sockaddr_un*>(&other.storage_);
                return std::strcmp(a->sun_path, b->sun_path) == 0;
            }
            default:
                return true;
        }
    }

    bool operator!=(const Address& other) const noexcept {
        return !(*this == other);
    }

private:
    struct sockaddr_storage storage_{};
    socklen_t len_ = 0;
    Family family_ = Family::UNSPEC;

    void clear() noexcept {
        std::memset(&storage_, 0, sizeof(storage_));
        len_ = 0;
        family_ = Family::UNSPEC;
    }
};

} // namespace Socket
