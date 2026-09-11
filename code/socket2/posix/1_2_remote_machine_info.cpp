#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

void get_remote_machine_info() {
    const std::string remote_host = "www.python.org";
    
    struct addrinfo hints, *res, *p;
    int status;
    char ip_address[INET_ADDRSTRLEN];
    
    // 设置hints结构
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // 允许IPv4或IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    
    // 解析主机名
    if ((status = getaddrinfo(remote_host.c_str(), NULL, &hints, &res)) != 0) {
        std::cerr << remote_host << ": " << gai_strerror(status) << std::endl;
        return;
    }
    
    // 遍历所有IP地址，获取第一个
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        std::string ip_version;
        
        // 获取IP地址指针
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ip_version = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ip_version = "IPv6";
        }
        
        // 将IP地址转换为字符串
        inet_ntop(p->ai_family, addr, ip_address, sizeof(ip_address));
        std::cout << "IP address of " << remote_host << " (" << ip_version << "): " 
                  << ip_address << std::endl;
        break; // 只取第一个
    }
    
    freeaddrinfo(res);
}

int main() {
    get_remote_machine_info();
    return 0;
}
