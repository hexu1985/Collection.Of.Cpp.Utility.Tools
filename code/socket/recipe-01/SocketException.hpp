#pragma once

#include <exception>
#include <string>
#include <string_view>

class SocketException: public std::exception {
public:
    SocketException(std::string_view msg, int error_code=0);
    ~SocketException() override;
    const char* what() const noexcept override;

    int ErrorCode() { return error_code_; }

private:
    virtual std::string get_error_code_detail() const;

protected:
    int error_code_=0;
    std::string msg_;
    mutable std::string what_;
};

class OSError: public SocketException {
public:
    OSError(std::string_view msg, int error_code=0);
    ~OSError() override;

private:
    std::string get_error_code_detail() const override;
};

// gethostbyname error
class HError: public SocketException {
public:
    HError(std::string_view msg, int error_code=0);
    ~HError() override;

private:
    std::string get_error_code_detail() const override;
};

// getaddrinfo error
class GAIError: public SocketException {
public:
    GAIError(std::string_view msg, int error_code=0);
    ~GAIError() override;

private:
    std::string get_error_code_detail() const override;
};
