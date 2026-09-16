#include "socket.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace unpsock;

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << "<port>\n";
        exit(1);
    }

    int port = std::stoi(argv[2]);

    Socket listen_sock(Family::INET, AddrType::STREAM);

    return 0;
}
