// socket.hpp
#pragma once

#include "socket_address.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <utility>
#include <stdexcept>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Socket {

// ---------- 异常 ----------
class SocketError : public std::runtime_error {
public:
    explicit SocketError(const std::string& msg, int errnum = 0)
        : std::runtime_error(msg), errnum_(errnum) {}

    int errnum() const noexcept { return errnum_; }

private:
    int errnum_;
};

// ---------- Socket ----------
class Socket {
public:
    // ===== 构造 / 析构 =====

    // 默认构造：不创建 fd，延迟到 bind/connect 时按地址族创建
    Socket() noexcept = default;

    // 显式指定族和类型，立即创建 fd
    Socket(Family family, AddrType type) {
        create(detail::to_af(family), detail::to_socktype(type));
        family_ = family;
        type_ = type;
    }

    // 从已有 fd 构造（供 accept 内部使用）
    Socket(int fd, Family family, AddrType type) noexcept
        : fd_(fd), family_(family), type_(type) {}

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept
        : fd_(other.fd_), family_(other.family_), type_(other.type_) {
        other.fd_ = -1;
    }

    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            close();
            fd_ = other.fd_;
            family_ = other.family_;
            type_ = other.type_;
            other.fd_ = -1;
        }
        return *this;
    }

    ~Socket() { close(); }

    // ===== 服务端 =====

    void bind(const Address& addr) {
        ensure_created(addr.family(),
                       type_ == AddrType::STREAM ? AddrType::STREAM : AddrType::DGRAM);
        if (::bind(fd_, addr.sockaddr_ptr(), addr.sockaddr_len()) != 0) {
            throw SocketError("bind failed: " + addr.to_string(), errno);
        }
    }

    void listen(int backlog = 5) {
        if (fd_ < 0) {
            throw SocketError("listen on uninitialized socket");
        }
        if (::listen(fd_, backlog) != 0) {
            throw SocketError("listen failed", errno);
        }
    }

    // 返回 (conn, peer_addr)，对应 Python 的 conn, addr = s.accept()
    std::pair<Socket, Address> accept() {
        if (fd_ < 0) {
            throw SocketError("accept on uninitialized socket");
        }

        struct sockaddr_storage storage;
        socklen_t len = sizeof(storage);
        int conn_fd = ::accept(fd_, reinterpret_cast<struct sockaddr*>(&storage), &len);
        if (conn_fd < 0) {
            throw SocketError("accept failed", errno);
        }

        Address peer = Address::from_sockaddr(
            reinterpret_cast<struct sockaddr*>(&storage), len);
        return {Socket(conn_fd, peer.family(), type_), std::move(peer)};
    }

    // ===== 客户端 =====

    void connect(const Address& addr) {
        ensure_created(addr.family(),
                       type_ == AddrType::STREAM ? AddrType::STREAM : AddrType::DGRAM);
        if (::connect(fd_, addr.sockaddr_ptr(), addr.sockaddr_len()) != 0) {
            throw SocketError("connect failed: " + addr.to_string(), errno);
        }
    }

    // 便捷重载：域名 + 端口，内部走 Address::resolve
    void connect(const std::string& host, uint16_t port,
                 Family family = Family::UNSPEC) {
        Address addr = Address::resolve(host, port, family,
                                        type_ == AddrType::STREAM
                                            ? AddrType::STREAM : AddrType::DGRAM);
        connect(addr);
    }

    // ===== 数据收发（TCP） =====

    // 返回实际收到的字节；对端关闭时返回空 vector
    std::vector<uint8_t> recv(size_t bufsize) {
        std::vector<uint8_t> buf(bufsize);
        ssize_t n = recv_impl(buf.data(), bufsize);
        if (n < 0) return {};
        buf.resize(static_cast<size_t>(n));
        return buf;
    }

    // 写入调用方缓冲区，返回实际字节数
    size_t recv(void* buf, size_t bufsize) {
        ssize_t n = recv_impl(buf, bufsize);
        if (n < 0) return 0;
        return static_cast<size_t>(n);
    }

    // 确保全部发出，对应 Python 的 sendall
    void sendall(const void* data, size_t len) {
        auto* p = static_cast<const uint8_t*>(data);
        size_t sent = 0;
        while (sent < len) {
            ssize_t n = ::send(fd_, p + sent, len - sent, MSG_NOSIGNAL);
            if (n < 0) {
                if (errno == EINTR) continue;
                throw SocketError("sendall failed", errno);
            }
            sent += static_cast<size_t>(n);
        }
    }

    void sendall(const std::vector<uint8_t>& data) {
        sendall(data.data(), data.size());
    }

    void sendall(const std::string& data) {
        sendall(data.data(), data.size());
    }

    // 对应 Python 的 send，返回实际发送字节数，可能少于 len
    size_t send(const void* data, size_t len) {
        ssize_t n = ::send(fd_, data, len, MSG_NOSIGNAL);
        if (n < 0) {
            throw SocketError("send failed", errno);
        }
        return static_cast<size_t>(n);
    }

    // ===== UDP =====

    std::pair<std::vector<uint8_t>, Address> recvfrom(size_t bufsize) {
        std::vector<uint8_t> buf(bufsize);
        struct sockaddr_storage storage;
        socklen_t len = sizeof(storage);

        ssize_t n = ::recvfrom(fd_, buf.data(), bufsize, 0,
                               reinterpret_cast<struct sockaddr*>(&storage), &len);
        if (n < 0) {
            throw SocketError("recvfrom failed", errno);
        }

        buf.resize(static_cast<size_t>(n));
        Address peer = Address::from_sockaddr(
            reinterpret_cast<struct sockaddr*>(&storage), len);
        return {std::move(buf), std::move(peer)};
    }

    size_t sendto(const void* data, size_t len, const Address& addr) {
        ssize_t n = ::sendto(fd_, data, len, 0,
                             addr.sockaddr_ptr(), addr.sockaddr_len());
        if (n < 0) {
            throw SocketError("sendto failed: " + addr.to_string(), errno);
        }
        return static_cast<size_t>(n);
    }

    size_t sendto(const std::vector<uint8_t>& data, const Address& addr) {
        return sendto(data.data(), data.size(), addr);
    }

    size_t sendto(const std::string& data, const Address& addr) {
        return sendto(data.data(), data.size(), addr);
    }

    // ===== 选项与状态 =====

    // 对应 Python 的 settimeout
    // nullopt = 阻塞，0 = 非阻塞，正数 = 超时秒数
    void settimeout(std::optional<double> seconds) {
        if (fd_ < 0) {
            throw SocketError("settimeout on uninitialized socket");
        }

        if (!seconds.has_value()) {
            // 恢复阻塞
            int flags = ::fcntl(fd_, F_GETFL, 0);
            if (flags < 0 || ::fcntl(fd_, F_SETFL, flags & ~O_NONBLOCK) < 0) {
                throw SocketError("settimeout(blocking) failed", errno);
            }
            struct timeval tv{0, 0};
            ::setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            ::setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
            return;
        }

        if (*seconds == 0.0) {
            // 非阻塞
            int flags = ::fcntl(fd_, F_GETFL, 0);
            if (flags < 0 || ::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) < 0) {
                throw SocketError("settimeout(nonblocking) failed", errno);
            }
            return;
        }

        // 正数超时：用 SO_RCVTIMEO / SO_SNDTIMEO
        struct timeval tv;
        tv.tv_sec = static_cast<time_t>(*seconds);
        tv.tv_usec = static_cast<suseconds_t>((*seconds - tv.tv_sec) * 1e6);
        if (::setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0 ||
            ::setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) != 0) {
            throw SocketError("settimeout failed", errno);
        }
    }

    // 对应 SO_REUSEADDR，服务端重启时避免 TIME_WAIT 报错
    void set_reuseaddr(bool enable = true) {
        if (fd_ < 0) {
            throw SocketError("set_reuseaddr on uninitialized socket");
        }
        int val = enable ? 1 : 0;
        if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) != 0) {
            throw SocketError("set_reuseaddr failed", errno);
        }
    }

    // 对应 SO_REUSEPORT（Linux），多进程/多线程负载均衡
    void set_reuseport(bool enable = true) {
        if (fd_ < 0) {
            throw SocketError("set_reuseport on uninitialized socket");
        }
        int val = enable ? 1 : 0;
        if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)) != 0) {
            throw SocketError("set_reuseport failed", errno);
        }
    }

    // 对应 getsockname
    Address getsockname() const {
        if (fd_ < 0) {
            throw SocketError("getsockname on uninitialized socket");
        }
        struct sockaddr_storage storage;
        socklen_t len = sizeof(storage);
        if (::getsockname(fd_, reinterpret_cast<struct sockaddr*>(&storage), &len) != 0) {
            throw SocketError("getsockname failed", errno);
        }
        return Address::from_sockaddr(
            reinterpret_cast<struct sockaddr*>(&storage), len);
    }

    // 对应 getpeername
    Address getpeername() const {
        if (fd_ < 0) {
            throw SocketError("getpeername on uninitialized socket");
        }
        struct sockaddr_storage storage;
        socklen_t len = sizeof(storage);
        if (::getpeername(fd_, reinterpret_cast<struct sockaddr*>(&storage), &len) != 0) {
            throw SocketError("getpeername failed", errno);
        }
        return Address::from_sockaddr(
            reinterpret_cast<struct sockaddr*>(&storage), len);
    }

    // 对应 close
    void close() noexcept {
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
    }

    // 对应 shutdown(SHUT_WR / SHUT_RDWR)
    void shutdown_write() {
        if (fd_ >= 0) ::shutdown(fd_, SHUT_WR);
    }

    void shutdown_both() {
        if (fd_ >= 0) ::shutdown(fd_, SHUT_RDWR);
    }

    bool closed() const noexcept { return fd_ < 0; }

    // 暴露底层 fd，便于与 select/poll/epoll 集成
    int fileno() const noexcept { return fd_; }

    Family family() const noexcept { return family_; }
    AddrType type() const noexcept { return type_; }

private:
    int fd_ = -1;
    Family family_ = Family::UNSPEC;
    AddrType type_ = AddrType::STREAM;

    void create(int af, int socktype) {
        fd_ = ::socket(af, socktype, 0);
        if (fd_ < 0) {
            throw SocketError("socket creation failed", errno);
        }
    }

    // 如果 fd 未创建，按地址族和类型创建；已创建则检查族是否匹配
    void ensure_created(Family family, AddrType type) {
        if (fd_ < 0) {
            create(detail::to_af(family), detail::to_socktype(type));
            family_ = family;
            type_ = type;
        } else if (family_ != Family::UNSPEC && family_ != family) {
            throw SocketError("socket family mismatch");
        }
    }

    ssize_t recv_impl(void* buf, size_t bufsize) {
        if (fd_ < 0) {
            throw SocketError("recv on uninitialized socket");
        }
        while (true) {
            ssize_t n = ::recv(fd_, buf, bufsize, 0);
            if (n < 0 && errno == EINTR) continue;
            if (n < 0) {
                throw SocketError("recv failed", errno);
            }
            return n;  // n == 0 表示对端关闭
        }
    }
};

} // namespace Socket
