#pragma once

#include <string>

namespace soa_on_dds {

class EprosimaRpcUtility {
public:
    static std::string generate_rpc_request_topic(const std::string& service_name);
    static std::string generate_rpc_response_topic(const std::string& service_name);
};

}   // namespace soa_on_dds
