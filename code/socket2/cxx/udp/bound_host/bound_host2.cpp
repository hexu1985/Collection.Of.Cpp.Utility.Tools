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

    Socket sock(Family::INET, AddrType::DGRAM);

    auto your_adr = Address::from_ip(argv[1], std::stoi(argv[2]));

    sock.sendto(msg1, your_adr);
    sock.sendto(msg2, your_adr);
    sock.sendto(msg3, your_adr);

    sock.close();

    return 0;
}

