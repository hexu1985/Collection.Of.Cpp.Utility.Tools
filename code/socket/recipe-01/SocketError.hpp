#pragma once

#include <exception>
#include <string>
#include <string_view>

class SocketError: public std::exception {
public:
    SocketError(int error_code, std::string_view msg);
    SocketError(std::string_view msg);

    int ErrorCode() { return error_code_; }

    const char* what() const noexcept override {
        return what_msg_.c_str();
    }

protected:
    SocketError(int error_code=0);

protected:
    int error_code_=0;
    std::string what_msg_;
};

// getaddrinfo error
class GAIError: public SocketError {
public:
    GAIError(int error_code, std::string_view msg);
};
