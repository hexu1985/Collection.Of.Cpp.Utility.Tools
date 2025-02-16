#pragma once

#include <stdexcept>
#include <string_view>

class SocketError: public std::runtime_error {
public:
    SocketError(int error_code, std::string_view msg);
    SocketError(std::string_view msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};

// gethostbyname error
class HostError: public std::runtime_error {
public:
    HostError(int error_code, std::string_view msg);
    HostError(std::string_view msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};

// getaddrinfo error
class AddrInfoError: public std::runtime_error {
public:
    AddrInfoError(int error_code, std::string_view msg);
    AddrInfoError(std::string_view msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};
