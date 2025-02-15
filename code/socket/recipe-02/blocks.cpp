#include <stddef.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <tuple>

#include <gflags/gflags.h>

#include "Socket.hpp"

#if __cplusplus >= 202002L
#include <format>
using std::format;
#else
#define FMT_HEADER_ONLY
#include "fmt/format.h"
using fmt::format;
#endif

DEFINE_string(host, "127.0.0.1", "IP address the client sends to");
DEFINE_uint32(port, 1060, "TCP port number");
DEFINE_bool(client, false, "run as the client");

std::ostream& operator<< (std::ostream& out, const std::tuple<std::string, uint16_t>& address)
{
    out << "(" << std::get<0>(address) << ", " << std::get<1>(address) << ")";
    return out;
}

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "\nusage: " << prog << " [--help] [--client] [--host HOST] [--port PORT]\n\n"
        << "Transmit & receive blocks over TCP\n";
    return os.str();
}

std::string recvall(Socket& sock, size_t length) {
    std::string blocks;
    while (length != 0) {
        auto block = sock.Recv(length);
        if (block.empty()) {
            throw std::runtime_error(format("socket closed with {} bytes left in this block", length));
        }
        length -= block.length();
        blocks += block;
    }
    return blocks;
}

std::string get_block(Socket& sock) {
    auto data = recvall(sock, sizeof(uint32_t));
    uint32_t block_length = ntohl(*(reinterpret_cast<const uint32_t*>(data.data())));
    return recvall(sock, block_length);
}

void put_block(Socket& sock, const std::string& message) {
    uint32_t block_length = message.length();
    block_length = htonl(block_length);
    sock.Send(&block_length, sizeof(block_length));
    sock.Send(message);
}

void server(const std::tuple<std::string, uint16_t>& address) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    sock.Bind(address);
    sock.Listen(1);
    std::cout << "Run this script in another window with '--client' to connect\n";
    std::cout << "Listening at " << sock.Getsockname() << "\n";
    std::tuple<std::string, uint16_t> sockname;
    Socket sc = sock.Accept(&sockname);
    std::cout << "Accepted connection from " << sockname << "\n";
    sc.Shutdown(SHUT_WR);
    while (true) {
        auto block = get_block(sc);
        if (block.empty()) {
            break;
        }
        std::cout << "Block says: " << block << "\n";
    }
    sc.Close();
    sock.Close();
}

void client(const std::tuple<std::string, uint16_t>& address) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Connect(address);
    sock.Shutdown(SHUT_RD);
    put_block(sock, "Beautiful is better than ugly.");
    put_block(sock, "Explicit is better than implicit.");
    put_block(sock, "Simple is better than complex.");
    put_block(sock, "");
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
