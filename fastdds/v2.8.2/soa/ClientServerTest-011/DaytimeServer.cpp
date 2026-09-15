#include "DaytimeServer.h"

#include <chrono>
#include <ctime>

using namespace std;
using namespace std::placeholders;
using namespace soa_on_dds;

DaytimeServer::DaytimeServer()
{
}

DaytimeServer::~DaytimeServer()
{
    mp_rpc_server.reset();
}

void DaytimeServer::serve()
{
    cout << "Enter a number to stop the server: ";
    int aux;
    std::cin >> aux;
}

bool DaytimeServer::init()
{
    mp_rpc_server = EprosimaRpcServer::create_rpc_server("daytime.service", 3);

    mp_rpc_server->register_method<VoidType, std::string>("daytime", std::bind(&DaytimeServer::daytime_handle, this, _1, _2));

    return mp_rpc_server->start();
}

void DaytimeServer::daytime_handle(const VoidType& arg, std::string& res) {
    std::cout << "EprosimaServer::daytime_handle begin" << std::endl;
    (void) arg;
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm_buf{};                       // 每个线程用自己的缓冲区
    if (localtime_r(&t, &tm_buf) == nullptr) {   // 可重入版本
        std::cerr << "localtime_r failed\n";
        return;
    }

    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_buf);
    res = buf;
    std::cout << "EprosimaServer::daytime_handle end" << std::endl;
}

