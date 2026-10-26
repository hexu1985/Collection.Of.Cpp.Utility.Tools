#include "socket.hpp"

#include <iostream>
#include <string>
#include <chrono>

using namespace unpsock;

int main(int argc, char *argv[])
{
    std::cout << "Creating socket... " << std::ends;
    Socket sock(Family::INET, AddrType::STREAM);
    std::cout << "done." << std::endl;

    std::cout << "onnecting to remote host... " << std::ends;
    sock.connect_timed("www.google.com", 80, std::chrono::seconds(5));
    std::cout << "done." << std::endl;

    sock.close();
    return 0;
}
