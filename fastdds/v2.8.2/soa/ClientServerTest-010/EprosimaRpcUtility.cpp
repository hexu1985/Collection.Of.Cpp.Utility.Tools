#include "EprosimaRpcUtility.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <chrono>
#include <ctime>
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

uint64_t EprosimaRpcUtility::get_current_time_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count();
}

std::string EprosimaRpcUtility::ms_to_string(uint64_t ms) {
    std::chrono::milliseconds duration(ms);
    std::chrono::system_clock::time_point tp(duration);
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm* tm = std::gmtime(&time);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    int millis = ms % 1000;
    return std::string(buffer) + "." + std::to_string(millis);
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
