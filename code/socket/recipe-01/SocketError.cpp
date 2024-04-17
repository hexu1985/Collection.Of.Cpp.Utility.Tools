#include "SocketError.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <sstream>

SocketError::SocketError(int error_code, std::string_view msg): error_code_(error_code) {
    std::ostringstream os;
    os << msg;
    if (error_code) {
        os << ": " << strerror(error_code);
    }
    what_msg_ = os.str();
}

SocketError::SocketError(std::string_view msg): SocketError(0, msg) {}

SocketError::SocketError(int error_code): error_code_(error_code) {}

GAIError::GAIError(int error_code, std::string_view msg): SocketError(error_code) {
    std::ostringstream os;
    os << msg;
    if (error_code) {
        os << ": " << gai_strerror(error_code);
    }
    what_msg_ = os.str();
}

