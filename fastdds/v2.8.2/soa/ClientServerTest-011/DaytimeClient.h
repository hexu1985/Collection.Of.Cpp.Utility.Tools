#pragma once

#include "EprosimaRpcClient.hpp"

#include <memory>

class DaytimeClient
{
public:

    DaytimeClient();

    virtual ~DaytimeClient();

    bool init();

    std::string get_daytime();

    bool isReady();

private:
    bool init_participant(); 
    bool init_rpc_client(); 

    std::shared_ptr<soa_on_dds::EprosimaRpcClient> mp_rpc_client;
};
