#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

class SocketError: public std::runtime_error {
public:
    SocketError(int error_code, std::string_view msg);
    SocketError(std::string_view msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};

// getaddrinfo error
class GAIError: public std::runtime_error {
public:
    GAIError(int error_code, std::string_view msg);
    GAIError(std::string_view msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};
