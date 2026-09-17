#pragma once

#include <cerrno>
#include <cstring>
#include <string>
#include <system_error>
#include <netdb.h>

namespace unpsock {

// ============================================================
// getaddrinfo 错误码的 error_category
// ============================================================
namespace detail {

class GaiCategory : public std::error_category {
public:
    const char* name() const noexcept override {
        return "getaddrinfo";
    }

    std::string message(int ev) const override {
        const char* s = ::gai_strerror(ev);
        return s ? std::string(s) : std::string("unknown getaddrinfo error");
    }
};

} // namespace detail

inline const std::error_category& gai_category() noexcept {
    static detail::GaiCategory instance;
    return instance;
}

inline std::error_code make_gai_error_code(int ev) noexcept {
    return std::error_code(ev, gai_category());
}

// ============================================================
// 异常基类
// ============================================================
class SocketException : public std::system_error {
public:
    SocketException(int errnum, const std::string& what)
        : std::system_error(errnum, std::system_category(), what) {}

    SocketException(std::error_code ec, const std::string& what)
        : std::system_error(ec, what) {}

    int errnum() const noexcept { return code().value(); }

    std::string errmsg() const { return code().message(); }
};

class AddressError : public SocketException {
public:
    explicit AddressError(const std::string& what, int gai_errnum = 0)
        : SocketException(make_gai_error_code(gai_errnum), what) {}

    explicit AddressError(std::error_code ec, const std::string& what)
        : SocketException(ec, what) {}
};

class SocketError : public SocketException {
public:
    explicit SocketError(const std::string& what, int errnum = 0)
        : SocketException(errnum, what) {}

    explicit SocketError(std::error_code ec, const std::string& what)
        : SocketException(ec, what) {}
};

} // namespace unpsock
