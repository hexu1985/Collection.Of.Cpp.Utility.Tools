#pragma once

#include <string>

class EprosimaRpcUtility {
public:
    static std::string generate_rpc_request_topic(const std::string& service_name);
    static std::string generate_rpc_response_topic(const std::string& service_name);
};
