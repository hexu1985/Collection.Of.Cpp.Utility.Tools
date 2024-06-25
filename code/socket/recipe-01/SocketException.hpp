#pragma once

#include <stdexcept>
#include <string>

#if __cplusplus >= 201703L
#include <string_view>
using StringView = std::string_view;
#else
#include <absl/strings/string_view.h>
using StringView = absl::string_view;
#endif

class SocketError: public std::runtime_error {
public:
    SocketError(int error_code, StringView msg);
    SocketError(StringView msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};

// getaddrinfo error
class GAIError: public std::runtime_error {
public:
    GAIError(int error_code, StringView msg);
    GAIError(StringView msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};
