#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

void print_machine_info() {
    char hostname[256];
    char ip_address[INET_ADDRSTRLEN];
    
    // 获取主机名
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        std::cerr << "Error getting hostname" << std::endl;
        return;
    }
    
    // 获取主机信息
    struct hostent *host_entry = gethostbyname(hostname);
    if (host_entry == nullptr) {
        std::cerr << "Error getting host information" << std::endl;
        return;
    }
    
    // 获取IP地址（第一个IP地址）
    struct in_addr *addr = (struct in_addr*)host_entry->h_addr_list[0];
    inet_ntop(AF_INET, addr, ip_address, sizeof(ip_address));
    
    // 输出信息
    std::cout << "Host name: " << hostname << std::endl;
    std::cout << "IP address: " << ip_address << std::endl;
}

int main() {
    print_machine_info();
    return 0;
}
