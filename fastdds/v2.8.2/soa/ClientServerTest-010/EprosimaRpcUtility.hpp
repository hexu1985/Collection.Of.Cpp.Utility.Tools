#pragma once

#include <cstdint>
#include <string>
#include <fastdds/dds/domain/DomainParticipant.hpp>

class EprosimaRpcUtility {
public:
    static std::string generate_rpc_request_topic(const std::string& service_name);
    static std::string generate_rpc_response_topic(const std::string& service_name);

    static std::string generate_rpc_client_id(const std::string& client_name);
    static long generate_rpc_session_id();

    static uint64_t get_current_time_ms();
    static std::string ms_to_string(uint64_t ms);

    using ParticipantPtr = eprosima::fastdds::dds::DomainParticipant*;
    static ParticipantPtr get_default_rpc_participant();

    static ParticipantPtr create_participant(
        eprosima::fastdds::dds::DomainId_t domain_id,
        const eprosima::fastdds::dds::DomainParticipantQos& qos,
        eprosima::fastdds::dds::DomainParticipantListener* listen=nullptr); 

    static void delete_participant(ParticipantPtr part);
};
