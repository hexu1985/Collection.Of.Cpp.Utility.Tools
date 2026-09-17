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

    const std::string message = "Hello World!";

    Socket serv_sock(Family::INET, AddrType::STREAM);

    auto serv_addr = Address::any(std::atoi(argv[1]));
    serv_sock.bind(serv_addr);
    serv_sock.listen(5);

    auto [clnt_sock, clnt_addr] = serv_sock.accept();

    clnt_sock.sendall(message);

    clnt_sock.close();
    serv_sock.close();

    return 0;
}
