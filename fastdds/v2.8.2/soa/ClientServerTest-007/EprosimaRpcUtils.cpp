#include "EprosimaRpcUtils.hpp"

#include <unistd.h>
#include <sys/types.h>

namespace {

const char* rpc_common_prefix = "soa.rpc.";
const char* rpc_request_suffix = ".request";
const char* rpc_response_suffix = ".response";

std::string get_host_name() {
    char hostname[256] = {0};

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        return "unknown";
    }

    return hostname;
}

}   // namespace

std::string EprosimaRpcUtility::generate_rpc_request_topic(const std::string& service_name) {
    return rpc_common_prefix+service_name+rpc_request_suffix;
}

std::string EprosimaRpcUtility::generate_rpc_response_topic(const std::string& service_name) {
    return rpc_common_prefix+service_name+rpc_response_suffix;
}

std::string EprosimaRpcUtility::generate_rpc_client_id(const std::string& client_name) {
    return client_name+"@"+get_host_name();
}

long EprosimaRpcUtility::generate_rpc_session_id() {
    return static_cast<long>(getpid());
}

