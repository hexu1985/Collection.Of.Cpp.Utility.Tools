#include "DDSParticipant.hpp"

#include <unistd.h>

#include <string>
#include <sstream>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;

namespace {

std::string get_host_name() {
    char hostname[256] = {0};

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        return "null";
    }

    return hostname;
}

std::string generate_participant_qos_name() {
    std::ostringstream os;
    os << "Participant_" << (int) getpid() << "@" << get_host_name();
    return os.str();
}

std::string to_string(const GUID_t& guid) {
    std::stringstream os;
    os << guid;
    return os.str();
}

std::string get_guid(DomainParticipant* participant) {
    if (participant == nullptr) {
        return "null";
    }

    return to_string(participant->guid());
}

}   // namespace

namespace fdds {

int DDSParticipant::getDomainId() {
    int domain_id = 0;
    const char* domain_id_str = getenv(FDDS_DOMAIN_ID);
    if (domain_id_str != nullptr) {
        domain_id = atoi(domain_id_str);
    }
    return domain_id;
}

DDSParticipant::DDSParticipant() {
    DomainParticipantQos pqos = PARTICIPANT_QOS_DEFAULT;

    auto factory = DomainParticipantFactory::get_instance();
    factory->load_profiles();
    if(ReturnCode_t::RETCODE_OK != factory->get_participant_qos_from_profile("participant_profile_1", pqos)) {
        logWarning(DDS_PARTICIPANT, "get_participant_qos_from_profile failed, use default qos");
    }
    pqos.name(generate_participant_qos_name());

    DomainId_t domain_id = getDomainId();
    participant_ = factory->create_participant(domain_id, pqos, &listener_, StatusMask::none());
    if (participant_ == nullptr) {
        logError(DDS_PARTICIPANT, "create_participant failed");
        return;
    }
}

DDSParticipant::~DDSParticipant() {
    if (participant_ == nullptr) {
        return;
    }

    auto factory = DomainParticipantFactory::get_instance();
    factory->delete_participant(participant_);
    participant_ = nullptr;
}

DomainParticipant* DDSParticipant::getDomainParticipant() {
    return participant_;
}

DDSParticipant& DDSParticipant::getInstance() {
    static DDSParticipant instance;
    return instance;
}

DDSParticipant::ParticipantListener::ParticipantListener() : DomainParticipantListener() {
}

DDSParticipant::ParticipantListener::~ParticipantListener() {
}

void DDSParticipant::ParticipantListener::on_participant_discovery(
        DomainParticipant* participant, 
        ParticipantDiscoveryInfo&& info)
{
    std::string participant_guid = get_guid(participant);
    std::string discovery_guid = to_string(info.info.m_guid);

    if (info.status == ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT)
    {
        logWarning(DDS_PARTICIPANT, "New participant discovered :PARTICIPANT[" << participant_guid << "] :DISCOVERY_PARTICIPANT[" << discovery_guid << "]");
    }
    else if (info.status == ParticipantDiscoveryInfo::REMOVED_PARTICIPANT ||
            info.status == ParticipantDiscoveryInfo::DROPPED_PARTICIPANT)
    {
        logWarning(DDS_PARTICIPANT, "New participant lost :PARTICIPANT[" << participant_guid << "] :DISCOVERY_PARTICIPANT[" << discovery_guid << "]");
    }
}

void DDSParticipant::ParticipantListener::on_subscriber_discovery(
        DomainParticipant* participant,
        ReaderDiscoveryInfo&& info)
{
    std::string participant_guid = get_guid(participant);
    std::string discovery_guid = to_string(info.info.guid());

    if (info.status == ReaderDiscoveryInfo::DISCOVERED_READER)
    {
        logWarning(DDS_PARTICIPANT, "New subscriber discovered :PARTICIPANT[" << participant_guid << "] :DISCOVERY_SUBSCRIBER[" << discovery_guid << "]");
    }
    else if (info.status == ReaderDiscoveryInfo::REMOVED_READER)
    {
        logWarning(DDS_PARTICIPANT, "New subscriber lost :PARTICIPANT[" << participant_guid << "] :DISCOVERY_SUBSCRIBER[" << discovery_guid << "]");
    }
}

void DDSParticipant::ParticipantListener::on_publisher_discovery(
        DomainParticipant* participant,
        WriterDiscoveryInfo&& info)
{
    std::string participant_guid = get_guid(participant);
    std::string discovery_guid = to_string(info.info.guid());

    if (info.status == WriterDiscoveryInfo::DISCOVERED_WRITER)
    {
        logWarning(DDS_PARTICIPANT, "New publisher discovered :PARTICIPANT[" << participant_guid << "] :DISCOVERY_PUBLISHER[" << discovery_guid << "]");
    }
    else if (info.status == eprosima::fastrtps::rtps::WriterDiscoveryInfo::REMOVED_WRITER)
    {
        logWarning(DDS_PARTICIPANT, "New publisher lost :PARTICIPANT[" << participant_guid << "] :DISCOVERY_PUBLISHER[" << discovery_guid << "]");
    }
}

}   // namespace
