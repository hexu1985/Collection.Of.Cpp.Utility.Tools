#include "socket.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace unpsock;

int main(int argc, char* argv[])
{
	if(argc!=2){
        std::cerr << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

    Socket serv_sock(Family::INET, AddrType::DGRAM);

    auto serv_adr = Address::any(std::atoi(argv[1]));
    serv_sock.bind(serv_adr);

    const int BUF_SIZE = 30;
    while (true) {
        auto [message, clnt_adr] = serv_sock.recvfrom(BUF_SIZE);
        serv_sock.sendto(message, clnt_adr);
    }

    serv_sock.close();

    return 0;
}

