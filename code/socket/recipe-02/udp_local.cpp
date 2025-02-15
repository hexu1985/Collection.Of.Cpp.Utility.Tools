#include <stddef.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <tuple>
#include <map>
#include <chrono>

#include <gflags/gflags.h>

#include "Socket.hpp"

#if __cplusplus >= 202002L
#include <format>
using std::format;
#else
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/ranges.h"
#include "fmt/chrono.h"
using fmt::format;
#endif

const size_t MAX_BYTES = 65535;

DEFINE_uint32(port, 1060, "TCP port number");
DEFINE_string(role, "", "which role to play");

std::ostream& operator<< (std::ostream& out, const std::tuple<std::string, uint16_t>& address)
{
    out << "(" << std::get<0>(address) << ", " << std::get<1>(address) << ")";
    return out;
}

static bool ValidateRole(const char* name, const std::string& value) {
    if (value == "client" || value == "server") {
        return true;
    }
    std::cout << "Invalid value for --" << name << ": " << value << "\n";
    std::cout << "  valid value: client or server\n";
    return false;
}

// 使用全局 static 变量来注册函数，static 变量会在 main 函数开始时就调用
static const bool role_dummy = gflags::RegisterFlagValidator(&FLAGS_role, &ValidateRole);

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--help] [--role client|server] [--port PORT]\n\n"
        << "Send and receive UDP locally\n";
    return os.str();
}

void server(uint16_t port) {
    Socket sock(AF_INET, SOCK_DGRAM);
    sock.Bind("127.0.0.1", port);
    std::cout << "Listening at " << sock.Getsockname() << "\n";
    SocketAddress address;
    std::string data;
    while (true) {
        data = sock.Recvfrom(MAX_BYTES, address);
        std::cout << "The client at " << address.to_string() << " says " << data << "\n";
        data = format("Your data was {} bytes long", data.length());
        sock.Sendto(data, address);
    }
}

void client(uint16_t port) {
    Socket sock(AF_INET, SOCK_DGRAM);
    std::string data = format("The time is {}", std::chrono::system_clock::now());
    sock.Sendto(data, "127.0.0.1", port);
    std::cout << "The OS assigned me the address " << sock.Getsockname() << "\n";
    SocketAddress address;
    data = sock.Recvfrom(MAX_BYTES, address);
    std::cout << "The server " << address.to_string() << " replied " << data << "\n";
    sock.Close();
}

int main(int argc, char* argv[]) {
    std::map<std::string, std::function<void(uint16_t)>> choices = {
        {"client", &client}, {"server", &server}
    };

    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    auto function = choices[FLAGS_role];
    function(FLAGS_port);

    return 0;
}
