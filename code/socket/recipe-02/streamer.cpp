#include <stddef.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <tuple>

#include <gflags/gflags.h>

#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/ranges.h"

#include "Socket.hpp"

using fmt::format;
using fmt::print;

DEFINE_string(host, "127.0.0.1", "IP address the client sends to");
DEFINE_uint32(port, 1060, "TCP port number");
DEFINE_bool(client, false, "run as the client");

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--help] [--client] [--host HOST] [--port PORT]\n\n"
        << "Transmit & receive a data stream\n";
    return os.str();
}

void server(const std::tuple<std::string, uint16_t>& address) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    sock.Bind(address);
    sock.Listen(1);
    print("Run this script in another window with '--client' to connect\n");
    print("Listening at {}\n", sock.Getsockname());
    std::tuple<std::string, uint16_t> sockname;
    Socket sc = sock.Accept(&sockname);
    print("Accepted connection from {}\n", sockname);
    sc.Shutdown(SHUT_WR);
    std::string message;
    while (true) {
        std::string more = sc.Recv(8192);
        if (more.empty()) {
            print("Received zero bytes - end of file\n");
            break;
        }
        print("Received {} bytes\n", more.length());
        message += more;
    }
    print("Message:\n");
    print(message);
    sc.Close();
    sock.Close();
}

void client(const std::tuple<std::string, uint16_t>& address) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Connect(address);
    sock.Shutdown(SHUT_RD);
    sock.sendall("Beautiful is better than ugly.\n");
    sock.sendall("Explicit is better than implicit.\n");
    sock.sendall("Simple is better than complex.\n");
    sock.Close();
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::function<void(const std::tuple<std::string, uint16_t>&)> function;
    function = FLAGS_client ? &client : &server;
    function(std::make_tuple(FLAGS_host, FLAGS_port));

    return 0;
}
