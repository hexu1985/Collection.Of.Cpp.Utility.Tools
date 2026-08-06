#include "EprosimaRpcUtils.hpp"

namespace {

const char* rpc_common_prefix = "soa.rpc.";
const char* rpc_request_suffix = ".request";
const char* rpc_response_suffix = ".response";

}   // namespace

std::string EprosimaRpcUtility::generate_rpc_request_topic(const std::string& service_name) {
    return rpc_common_prefix+service_name+rpc_request_suffix;
}

std::string EprosimaRpcUtility::generate_rpc_response_topic(const std::string& service_name) {
    return rpc_common_prefix+service_name+rpc_response_suffix;
}
