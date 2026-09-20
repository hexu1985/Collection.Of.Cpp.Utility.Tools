#pragma once

#include <cstdint>
#include <string>
#include <fastdds/dds/domain/DomainParticipant.hpp>

#include "soa_on_dds_types.h"

namespace soa_on_dds {

class EprosimaRpcUtility {
public:
    static std::string generate_rpc_request_topic(const std::string& service_name);
    static std::string generate_rpc_response_topic(const std::string& service_name);

    static std::string generate_rpc_client_id(const std::string& client_name);
    static long generate_rpc_session_id();

    static uint64_t get_current_time_ms();
    static std::string ms_to_string(uint64_t ms);

    static std::string error_code_to_string(soa_on_dds::ErrorCode code);

    using ParticipantPtr = eprosima::fastdds::dds::DomainParticipant*;
    static ParticipantPtr get_default_rpc_participant();

    static ParticipantPtr create_participant(
        eprosima::fastdds::dds::DomainId_t domain_id,
        const eprosima::fastdds::dds::DomainParticipantQos& qos,
        eprosima::fastdds::dds::DomainParticipantListener* listen=nullptr); 

    static void delete_participant(ParticipantPtr part);
};

struct VoidType {
};

inline
bool SerializeToVector(const VoidType&, std::vector<uint8_t>& output) {
    output.clear();
    return true;
}

inline
bool DeserializeFromVector(VoidType&, const std::vector<uint8_t>&) {
    return true;
}

inline
bool SerializeToVector(const std::string& str, std::vector<uint8_t>& output) {
    output.assign(str.begin(), str.end());
    return true;
}

inline
bool DeserializeFromVector(std::string& str, const std::vector<uint8_t>& input) {
    str.assign(input.begin(), input.end());
    return true;
}

}   // namespace soa_on_dds
