#include "socket.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace unpsock;

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <IPaddress> <port>\n";
        exit(1);
    }

    std::string host = argv[1];
    int port = std::stoi(argv[2]);

    Socket sock(Family::INET, AddrType::STREAM);
    sock.connect(host, port);

    while (true) {
        auto data = sock.recv(1024);
        if (data.empty()) {
            break;
        }
        std::cout << std::string(data.begin(), data.end());
    }
    std::cout << std::ends;

    sock.close();

    return 0;
}
