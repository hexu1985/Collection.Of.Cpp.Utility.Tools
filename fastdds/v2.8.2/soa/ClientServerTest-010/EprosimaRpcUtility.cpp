#include "EprosimaRpcUtility.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <unistd.h>
#include <sys/types.h>

using namespace eprosima::fastdds::dds;

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

class DefaultRpcParticipant {
public:
    DefaultRpcParticipant() {
        DomainParticipantQos pqos;
        pqos.wire_protocol().builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
        pqos.wire_protocol().builtin.discovery_config.discoveryProtocol =
            eprosima::fastrtps::rtps::DiscoveryProtocol::SIMPLE;
        pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
        pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
        pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
        pqos.name("DefaultRpcParticipant");

        const eprosima::fastdds::dds::DomainId_t rpc_domain_id = 40;
        m_participant = EprosimaRpcUtility::create_participant(rpc_domain_id, pqos);
    }

    ~DefaultRpcParticipant() {
        if (m_participant != nullptr) {
            EprosimaRpcUtility::delete_participant(m_participant);
        }
    }

    eprosima::fastdds::dds::DomainParticipant* m_participant=nullptr;
};

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

EprosimaRpcUtility::ParticipantPtr EprosimaRpcUtility::get_default_rpc_participant() {
    static DefaultRpcParticipant default_rpc_participant;
    return default_rpc_participant.m_participant;
}

EprosimaRpcUtility::ParticipantPtr 
EprosimaRpcUtility::create_participant(DomainId_t domain_id,
    const DomainParticipantQos& qos, DomainParticipantListener* listen) {
    auto participant = DomainParticipantFactory::get_instance()->create_participant(domain_id, qos, listen, StatusMask::none());
    if (participant == nullptr) {
        return nullptr;
    }

    return participant;
}

void EprosimaRpcUtility::delete_participant(ParticipantPtr part) {
    DomainParticipantFactory::get_instance()->delete_participant(part);
}
