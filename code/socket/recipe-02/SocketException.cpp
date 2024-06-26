#include "SocketException.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <sstream>

namespace {

std::string Strerror(StringView msg, int error_code) {
    std::ostringstream os;
    os << msg;
    if (error_code) {
        os << ": " << strerror(error_code);
    }
    return os.str();
}

std::string Gai_strerror(StringView msg, int error_code) {
    std::ostringstream os;
    os << msg;
    if (error_code) {
        os << ": " << gai_strerror(error_code);
    }
    return os.str();
}

}   // namespace

SocketError::SocketError(int error_code, StringView msg): 
    std::runtime_error(Strerror(msg, error_code)), 
    error_code_(error_code) {
}

SocketError::SocketError(StringView msg): SocketError(0, msg) {}

GAIError::GAIError(int error_code, StringView msg): 
    std::runtime_error(Gai_strerror(msg, error_code)),
    error_code_(error_code) {
}

GAIError::GAIError(StringView msg): GAIError(0, msg) {}

