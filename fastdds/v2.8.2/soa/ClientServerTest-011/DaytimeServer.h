#pragma once

#include "EprosimaRpcServer.hpp"

class DaytimeServer
{
public:

    DaytimeServer();

    virtual ~DaytimeServer();

    bool init();

    //Serve indefinitely.
    void serve();

    void daytime_handle(const soa_on_dds::VoidType& arg,
            std::string& res);

private:
    std::shared_ptr<soa_on_dds::EprosimaRpcServer> mp_rpc_server;
};
