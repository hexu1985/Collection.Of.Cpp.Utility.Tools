#include "socket.hpp"

#include <iostream>

using namespace unpsock;

int main(int argc, char* argv[])
{
    if(argc!=3){
        std::cerr << "Usage: " << argv[0] << " <IP> <port>\n";
        exit(1);
    }

    std::string msg1="Hi!";
    std::string msg2="I'm another UDP host!";
    std::string msg3="Nice to meet you";

    Socket sock(Family::INET, AddrType::STREAM);

    auto serv_adr = Address::from_ip(argv[1], std::stoi(argv[2]));
    sock.connect(serv_adr);

    sock.send(msg1);
    sock.send(msg2);
    sock.send(msg3);

    sock.close();

    return 0;
}

