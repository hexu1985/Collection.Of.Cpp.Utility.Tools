#include "DaytimeClient.h"

#include <string>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;

DaytimeClient::DaytimeClient()
{
}

DaytimeClient::~DaytimeClient()
{
    mp_rpc_client.reset();
}

bool DaytimeClient::init()
{
    //std::cout << "DaytimeClient::init_rpc_client" << std::endl;
    mp_rpc_client = soa_on_dds::EprosimaRpcClient::create_rpc_client("daytime_client", "daytime.service");
    return mp_rpc_client->init();
}

std::string DaytimeClient::get_daytime() {
    std::string res;
    auto error_code = mp_rpc_client->call("daytime", soa_on_dds::VoidType{}, res);
    if (error_code != soa_on_dds::SUCCESS) {
        std::cout << "DaytimeClient::get_daytime error: " 
            << soa_on_dds::EprosimaRpcUtility::error_code_to_string(error_code) << std::endl;
        return {};
    }

    return res;
}

bool DaytimeClient::isReady() {
    if (mp_rpc_client == nullptr) {
        return false;
    }

    return mp_rpc_client->is_ready();
}
