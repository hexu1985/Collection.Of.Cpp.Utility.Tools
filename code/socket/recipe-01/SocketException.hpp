#pragma once

#include <stdexcept>
#include <string_view>

class OSError: public std::runtime_error {
public:
    OSError(int error_code, std::string_view msg);
    OSError(std::string_view msg);

    int ErrorCode() { return error_code_; }

protected:
    int error_code_=0;
};

// gethostbyname error
class HError: public std::runtime_error {
public:
    HError(int error_code, std::string_view msg);
    HError(std::string_view msg);

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
