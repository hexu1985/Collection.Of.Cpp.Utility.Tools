#include <stddef.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <tuple>
#include <map>
#include <chrono>

#include <gflags/gflags.h>

#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/ranges.h"
#include "fmt/chrono.h"

#include "Socket.hpp"

using fmt::format;
using fmt::print;

const size_t MAX_BYTES = 65535;

DEFINE_uint32(port, 1060, "TCP port number");
DEFINE_string(role, "", "which role to play");

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
    print("Listening at {}\n", sock.Getsockname());
    SocketAddress address;
    std::string data;
    while (true) {
        data = sock.Recvfrom(MAX_BYTES, address);
        print("The client at {} says {}\n", address.to_string(), data);
        data = format("Your data was {} bytes long", data.length());
        sock.Sendto(data, address);
    }
}

void client(uint16_t port) {
    Socket sock(AF_INET, SOCK_DGRAM);
    std::string data = format("The time is {}", std::chrono::system_clock::now());
    sock.Sendto(data, "127.0.0.1", port);
    print("The OS assigned me the address {}\n", sock.Getsockname());
    SocketAddress address;
    data = sock.Recvfrom(MAX_BYTES, address);
    print("The server {} replied {}\n", address.to_string(), data);
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
