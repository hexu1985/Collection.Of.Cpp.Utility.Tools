#pragma once

#include <stdexcept>
#include <string>
#include <absl/strings/string_view.h>

class SocketError: public std::runtime_error {
public:
    SocketError(int error_code, absl::string_view msg);
    SocketError(absl::string_view msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};

// getaddrinfo error
class GAIError: public std::runtime_error {
public:
    GAIError(int error_code, absl::string_view msg);
    GAIError(absl::string_view msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};
