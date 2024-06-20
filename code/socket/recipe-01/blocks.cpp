#include <stddef.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <tuple>

#include <gflags/gflags.h>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

#include "Socket.hpp"

using fmt::format;

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
    uint32_t block_length = ntohl(*(reinterpret_cast<uint32_t*>(data.data())));
    return recvall(sock, block_length);
}

template <typename T>
std::string_view make_string_view(const T* data) {
    return std::string_view(reinterpret_cast<const char*>(data), sizeof(T));
}

void put_block(Socket& sock, const std::string& message) {
    uint32_t block_length = message.length();
    block_length = htonl(block_length);
    sock.Send(make_string_view(&block_length));
    sock.Send(message);
}

void server(const char* host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    sock.Bind(host, port);
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

void client(const char* host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Connect(host, port);
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

    std::function<void(const char*, uint16_t)> function;
    if (FLAGS_client) {
        function = &client;
    } else {
        function = &server;
    }
    function(FLAGS_host.c_str(), FLAGS_port);

    return 0;
}
