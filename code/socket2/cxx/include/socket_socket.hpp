#pragma once

#include "socket_address.hpp"
#include "socket_exception.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Socket {

// ---------- Socket ----------
class Socket {
public:
    // ============================================================
    // 构造 / 析构
    // ============================================================
    Socket() noexcept = default;

    // ---- 异常版本 ----
    Socket(Family family, AddrType type) {
        create(detail::to_af(family), detail::to_socktype(type));
        family_ = family;
        type_ = type;
    }

    // ---- 错误码版本 ----
    Socket(Family family, AddrType type, std::error_code& ec) noexcept {
        ec.clear();
        create(detail::to_af(family), detail::to_socktype(type), ec);
        if (!ec) {
            family_ = family;
            type_ = type;
        }
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

    bool valid() const noexcept { return fd_ >= 0; }
    explicit operator bool() const noexcept { return fd_ >= 0; }

    // ============================================================
    // 服务端
    // ============================================================

    // ---- bind：异常版本 ----
    void bind(const Address& addr) {
        std::error_code ec;
        bind(addr, ec);
        if (ec) {
            throw SocketError(ec, "bind failed: " + addr.to_string());
        }
    }

    // ---- bind：错误码版本 ----
    void bind(const Address& addr, std::error_code& ec) noexcept {
        ec.clear();
        if (::bind(fd_, addr.sockaddr_ptr(), addr.sockaddr_len()) != 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    // ---- listen：异常版本 ----
    void listen(int backlog = 5) {
        std::error_code ec;
        listen(backlog, ec);
        if (ec) {
            throw SocketError(ec, "listen failed");
        }
    }

    // ---- listen：错误码版本 ----
    void listen(int backlog, std::error_code& ec) noexcept {
        ec.clear();
        if (fd_ < 0) {
            ec = std::make_error_code(std::errc::bad_file_descriptor);
            return;
        }
        if (::listen(fd_, backlog) != 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    // ---- accept：异常版本 ----
    std::pair<Socket, Address> accept() {
        std::error_code ec;
        auto r = accept(ec);
        if (ec) {
            throw SocketError(ec, "accept failed");
        }
        return r;
    }

    // ---- accept：错误码版本 ----
    std::pair<Socket, Address> accept(std::error_code& ec) noexcept {
        ec.clear();
        if (fd_ < 0) {
            ec = std::make_error_code(std::errc::bad_file_descriptor);
            return {Socket(), Address()};
        }

        struct sockaddr_storage storage;
        socklen_t len = sizeof(storage);

        int conn_fd;
        do {
            conn_fd = ::accept(fd_,
                               reinterpret_cast<struct sockaddr*>(&storage),
                               &len);
        } while (conn_fd < 0 && errno == EINTR);

        if (conn_fd < 0) {
            ec = std::error_code(errno, std::system_category());
            return {Socket(), Address()};
        }

        Address peer = Address::from_sockaddr(
            reinterpret_cast<struct sockaddr*>(&storage), len);
        return {Socket(conn_fd, peer.family(), type_), std::move(peer)};
    }

    // ============================================================
    // 客户端
    // ============================================================

    // ---- connect(Address)：异常版本 ----
    void connect(const Address& addr) {
        std::error_code ec;
        connect(addr, ec);
        if (ec) {
            throw SocketError(ec, "connect failed: " + addr.to_string());
        }
    }

    // ---- connect(Address)：错误码版本 ----
    void connect(const Address& addr, std::error_code& ec) noexcept {
        ec.clear();

        int rc;
        do {
            rc = ::connect(fd_, addr.sockaddr_ptr(), addr.sockaddr_len());
        } while (rc < 0 && errno == EINTR);

        if (rc != 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    // ---- connect(host, port)：异常版本 ----
    void connect(const std::string& host, uint16_t port,
                 Family family = Family::UNSPEC) {
        std::error_code ec;
        connect(host, port, family, ec);
        if (ec) {
            throw SocketError(ec, "connect failed: " + host + ":" + std::to_string(port));
        }
    }

    // ---- connect(host, port)：错误码版本 ----
    // 注意：resolve 失败时 ec 是 gai_category 的码
    void connect(const std::string& host, uint16_t port,
                 Family family, std::error_code& ec) noexcept {
        ec.clear();
        Address addr = Address::resolve(host, port, family, type_, ec);
        if (ec) return;
        connect(addr, ec);
    }

    // ============================================================
    // 数据收发（TCP）
    // ============================================================

    // ---- recv(size_t)：异常版本 ----
    std::vector<uint8_t> recv(size_t bufsize) {
        std::error_code ec;
        auto v = recv(bufsize, ec);
        if (ec) {
            throw SocketError(ec, "recv failed");
        }
        return v;
    }

    // ---- recv(size_t)：错误码版本 ----
    // 对端关闭时返回空 vector 且 ec 为空；出错时 ec 非空
    std::vector<uint8_t> recv(size_t bufsize, std::error_code& ec) noexcept {
        ec.clear();
        std::vector<uint8_t> buf(bufsize);
        ssize_t n = recv_impl(buf.data(), bufsize, ec);
        if (ec) return {};
        buf.resize(static_cast<size_t>(n));
        return buf;
    }

    // ---- recv(void*, size_t)：异常版本 ----
    size_t recv(void* buf, size_t bufsize) {
        std::error_code ec;
        size_t n = recv(buf, bufsize, ec);
        if (ec) {
            throw SocketError(ec, "recv failed");
        }
        return n;
    }

    // ---- recv(void*, size_t)：错误码版本 ----
    // 返回实际字节数；对端关闭返回 0 且 ec 为空；出错返回 0 且 ec 非空
    size_t recv(void* buf, size_t bufsize, std::error_code& ec) noexcept {
        ec.clear();
        ssize_t n = recv_impl(buf, bufsize, ec);
        if (ec) return 0;
        return static_cast<size_t>(n);
    }

    // ---- sendall：异常版本 ----
    void sendall(const void* data, size_t len) {
        std::error_code ec;
        sendall(data, len, ec);
        if (ec) {
            throw SocketError(ec, "sendall failed");
        }
    }

    void sendall(const std::vector<uint8_t>& data) {
        sendall(data.data(), data.size());
    }

    void sendall(const std::string& data) {
        sendall(data.data(), data.size());
    }

    // ---- sendall：错误码版本 ----
    void sendall(const void* data, size_t len, std::error_code& ec) noexcept {
        ec.clear();
        auto* p = static_cast<const uint8_t*>(data);
        size_t sent = 0;
        while (sent < len) {
            ssize_t n = ::send(fd_, p + sent, len - sent, MSG_NOSIGNAL);
            if (n < 0) {
                if (errno == EINTR) continue;
                ec = std::error_code(errno, std::system_category());
                return;
            }
            if (n == 0) {
                // 理论上阻塞 socket 不会返回 0，但防御性处理
                ec = std::make_error_code(std::errc::io_error);
                return;
            }
            sent += static_cast<size_t>(n);
        }
    }

    void sendall(const std::vector<uint8_t>& data, std::error_code& ec) noexcept {
        sendall(data.data(), data.size(), ec);
    }

    void sendall(const std::string& data, std::error_code& ec) noexcept {
        sendall(data.data(), data.size(), ec);
    }

    // ---- send：异常版本 ----
    size_t send(const void* data, size_t len) {
        std::error_code ec;
        size_t n = send(data, len, ec);
        if (ec) {
            throw SocketError(ec, "send failed");
        }
        return n;
    }

    // ---- send：错误码版本 ----
    size_t send(const void* data, size_t len, std::error_code& ec) noexcept {
        ec.clear();
        ssize_t n;
        do {
            n = ::send(fd_, data, len, MSG_NOSIGNAL);
        } while (n < 0 && errno == EINTR);

        if (n < 0) {
            ec = std::error_code(errno, std::system_category());
            return 0;
        }
        return static_cast<size_t>(n);
    }

    // ============================================================
    // UDP
    // ============================================================

    // ---- recvfrom：异常版本 ----
    std::pair<std::vector<uint8_t>, Address> recvfrom(size_t bufsize) {
        std::error_code ec;
        auto r = recvfrom(bufsize, ec);
        if (ec) {
            throw SocketError(ec, "recvfrom failed");
        }
        return r;
    }

    // ---- recvfrom：错误码版本 ----
    std::pair<std::vector<uint8_t>, Address>
    recvfrom(size_t bufsize, std::error_code& ec) noexcept {
        ec.clear();
        std::vector<uint8_t> buf(bufsize);
        struct sockaddr_storage storage;
        socklen_t len = sizeof(storage);

        ssize_t n;
        do {
            n = ::recvfrom(fd_, buf.data(), bufsize, 0,
                           reinterpret_cast<struct sockaddr*>(&storage), &len);
        } while (n < 0 && errno == EINTR);

        if (n < 0) {
            ec = std::error_code(errno, std::system_category());
            return {{}, Address()};
        }

        buf.resize(static_cast<size_t>(n));
        Address peer = Address::from_sockaddr(
            reinterpret_cast<struct sockaddr*>(&storage), len);
        return {std::move(buf), std::move(peer)};
    }

    // ---- sendto：异常版本 ----
    size_t sendto(const void* data, size_t len, const Address& addr) {
        std::error_code ec;
        size_t n = sendto(data, len, addr, ec);
        if (ec) {
            throw SocketError(ec, "sendto failed: " + addr.to_string());
        }
        return n;
    }

    size_t sendto(const std::vector<uint8_t>& data, const Address& addr) {
        return sendto(data.data(), data.size(), addr);
    }

    size_t sendto(const std::string& data, const Address& addr) {
        return sendto(data.data(), data.size(), addr);
    }

    // ---- sendto：错误码版本 ----
    size_t sendto(const void* data, size_t len, const Address& addr,
                  std::error_code& ec) noexcept {
        ec.clear();
        ssize_t n;
        do {
            n = ::sendto(fd_, data, len, 0,
                         addr.sockaddr_ptr(), addr.sockaddr_len());
        } while (n < 0 && errno == EINTR);

        if (n < 0) {
            ec = std::error_code(errno, std::system_category());
            return 0;
        }
        return static_cast<size_t>(n);
    }

    size_t sendto(const std::vector<uint8_t>& data, const Address& addr,
                  std::error_code& ec) noexcept {
        return sendto(data.data(), data.size(), addr, ec);
    }

    size_t sendto(const std::string& data, const Address& addr,
                  std::error_code& ec) noexcept {
        return sendto(data.data(), data.size(), addr, ec);
    }

    // ============================================================
    // 查询
    // ============================================================

    // ---- getsockname：异常版本 ----
    Address getsockname() const {
        std::error_code ec;
        Address a = getsockname(ec);
        if (ec) {
            throw SocketError(ec, "getsockname failed");
        }
        return a;
    }

    // ---- getsockname：错误码版本 ----
    Address getsockname(std::error_code& ec) const noexcept {
        ec.clear();
        if (fd_ < 0) {
            ec = std::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }
        struct sockaddr_storage storage;
        socklen_t len = sizeof(storage);
        if (::getsockname(fd_, reinterpret_cast<struct sockaddr*>(&storage),
                          &len) != 0) {
            ec = std::error_code(errno, std::system_category());
            return {};
        }
        return Address::from_sockaddr(
            reinterpret_cast<struct sockaddr*>(&storage), len);
    }

    // ---- getpeername：异常版本 ----
    Address getpeername() const {
        std::error_code ec;
        Address a = getpeername(ec);
        if (ec) {
            throw SocketError(ec, "getpeername failed");
        }
        return a;
    }

    // ---- getpeername：错误码版本 ----
    Address getpeername(std::error_code& ec) const noexcept {
        ec.clear();
        if (fd_ < 0) {
            ec = std::make_error_code(std::errc::bad_file_descriptor);
            return {};
        }
        struct sockaddr_storage storage;
        socklen_t len = sizeof(storage);
        if (::getpeername(fd_, reinterpret_cast<struct sockaddr*>(&storage),
                          &len) != 0) {
            ec = std::error_code(errno, std::system_category());
            return {};
        }
        return Address::from_sockaddr(
            reinterpret_cast<struct sockaddr*>(&storage), len);
    }

    // ============================================================
    // 关闭 / 半关闭
    // ============================================================
    void close() noexcept {
        if (fd_ >= 0) {
            // Linux close(2) 在 EINTR 时不重试（fd 可能已释放）
            ::close(fd_);
            fd_ = -1;
        }
    }

    // ---- shutdown_write：异常版本 ----
    void shutdown_write() {
        std::error_code ec;
        shutdown_write(ec);
        if (ec) {
            throw SocketError(ec, "shutdown_write failed");
        }
    }

    // ---- shutdown_write：错误码版本 ----
    void shutdown_write(std::error_code& ec) noexcept {
        ec.clear();
        if (fd_ < 0) return;
        if (::shutdown(fd_, SHUT_WR) != 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    // ---- shutdown_read：异常版本 ----
    void shutdown_read() {
        std::error_code ec;
        shutdown_read(ec);
        if (ec) {
            throw SocketError(ec, "shutdown_read failed");
        }
    }

    // ---- shutdown_read：错误码版本 ----
    void shutdown_read(std::error_code& ec) noexcept {
        ec.clear();
        if (fd_ < 0) return;
        if (::shutdown(fd_, SHUT_RD) != 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    // ---- shutdown_both：异常版本 ----
    void shutdown_both() {
        std::error_code ec;
        shutdown_both(ec);
        if (ec) {
            throw SocketError(ec, "shutdown_both failed");
        }
    }

    // ---- shutdown_both：错误码版本 ----
    void shutdown_both(std::error_code& ec) noexcept {
        ec.clear();
        if (fd_ < 0) return;
        if (::shutdown(fd_, SHUT_RDWR) != 0) {
            ec = std::error_code(errno, std::system_category());
        }
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

    // ---- create：异常版本 ----
    void create(int af, int socktype) {
        std::error_code ec;
        create(af, socktype, ec);
        if (ec) {
            throw SocketError(ec, "socket creation failed");
        }
    }

    // ---- create：错误码版本 ----
    void create(int af, int socktype,
                std::error_code& ec) noexcept {
        ec.clear();
        fd_ = ::socket(af, socktype, 0);
        if (fd_ < 0) {
            ec = std::error_code(errno, std::system_category());
            fd_ = -1;
        }
    }

    // 返回实际收到的字节数；对端关闭返回 0 且 ec 为空；出错返回 -1 且 ec 非空
    ssize_t recv_impl(void* buf, size_t bufsize,
                      std::error_code& ec) noexcept {
        ec.clear();
        if (fd_ < 0) {
            ec = std::make_error_code(std::errc::bad_file_descriptor);
            return -1;
        }
        while (true) {
            ssize_t n = ::recv(fd_, buf, bufsize, 0);
            if (n < 0 && errno == EINTR) continue;
            if (n < 0) {
                ec = std::error_code(errno, std::system_category());
                return -1;
            }
            return n;  // n == 0 表示对端关闭
        }
    }
};

} // namespace Socket