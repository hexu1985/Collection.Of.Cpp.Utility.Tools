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

    Socket serv_sock(Family::INET, AddrType::STREAM);

    auto serv_addr = Address::any(std::atoi(argv[1]));
    serv_sock.bind(serv_addr);
    serv_sock.listen(5);

    const int BUF_SIZE = 1024;
	for(int i=0; i<5; i++)
	{
        auto [clnt_sock, clnt_addr] = serv_sock.accept();
        std::cout << "Connected client " << i+1 << std::endl;

        while (true) {
            auto data = clnt_sock.recv(BUF_SIZE);
            if (data.empty()) {
                break;
            }
            clnt_sock.sendall(data);
        }
        clnt_sock.close();
    }

    serv_sock.close();

    return 0;
}

