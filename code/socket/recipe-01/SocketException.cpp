#include "SocketException.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <sstream>

SocketException::SocketException(std::string_view msg, int error_code): 
    error_code_(error_code), msg_(msg) {
}

SocketException::~SocketException() {
}

std::string SocketException::get_error_code_detail() const {
    return {};
}

const char* SocketException::what() const noexcept {
    if (error_code_ == 0) {
        return msg_.c_str();
    }
    if (what_.empty()) {
        std::ostringstream os;
        os << msg_ << ", error_code(" << error_code_ << ")"; 
        std::string error_code_detail = get_error_code_detail();
        if (!error_code_detail.empty()) {
            os << ": " << error_code_detail;
        }
    }
    return what_.c_str();
}

OSError::OSError(std::string_view msg, int error_code): 
    SocketException(msg, error_code) {
}

OSError::~OSError() {
}

std::string OSError::get_error_code_detail() const {
    std::string error_code_detail = strerror(error_code_);
    return error_code_detail;
}

HError::HError(std::string_view msg, int error_code): 
    SocketException(msg, error_code) {
}

HError::~HError() {
}

std::string HError::get_error_code_detail() const {
    std::string error_code_detail = hstrerror(error_code_);
    return error_code_detail;
}

GAIError::GAIError(std::string_view msg, int error_code): 
    SocketException(msg, error_code) {
}

GAIError::~GAIError() {
}

std::string GAIError::get_error_code_detail() const {
    std::string error_code_detail = gai_strerror(error_code_);
    return error_code_detail;
}

