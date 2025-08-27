#include "ParticipantListener.hpp"
#include <fastdds/dds/domain/DomainParticipant.hpp>

#include <iostream>
#include <sstream>
#include <string>

std::string ToString(const eprosima::fastrtps::rtps::GUID_t& guid) {
    std::stringstream os;
    os << guid;
    return os.str();
}

std::string get_guid(eprosima::fastdds::dds::DomainParticipant* participant) {
    if (participant == nullptr) {
        return "";
    }

    return ToString(participant->guid());
}

void ParticipantListener::on_participant_discovery(
        eprosima::fastdds::dds::DomainParticipant* participant,
        eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info)
{
    std::string participant_guid = get_guid(participant);
    std::string discovery_guid = ToString(info.info.m_guid);

    if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT)
    {
        std::cout << "New participant discovered :PARTICIPANT[" << participant_guid.c_str() << "] :DISCOVERY_PARTICIPANT[" << discovery_guid.c_str() << "]\n";
    }
    else if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::REMOVED_PARTICIPANT ||
            info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DROPPED_PARTICIPANT)
    {
        std::cout << "New participant lost :PARTICIPANT[" << participant_guid.c_str() << "] :DISCOVERY_PARTICIPANT[" << discovery_guid.c_str() << "]\n";
    }
}

void ParticipantListener::on_subscriber_discovery(
        eprosima::fastdds::dds::DomainParticipant* participant,
        eprosima::fastrtps::rtps::ReaderDiscoveryInfo&& info)
{
    std::string participant_guid = get_guid(participant);
    std::string discovery_guid = ToString(info.info.guid());

    if (info.status == eprosima::fastrtps::rtps::ReaderDiscoveryInfo::DISCOVERED_READER)
    {
        std::cout << "New subscriber discovered :PARTICIPANT[" << participant_guid.c_str() << "] :DISCOVERY_SUBSCRIBER[" << discovery_guid.c_str() << "]\n";
    }
    else if (info.status == eprosima::fastrtps::rtps::ReaderDiscoveryInfo::REMOVED_READER)
    {
        std::cout << "New subscriber lost :PARTICIPANT[" << participant_guid.c_str() << "] :DISCOVERY_SUBSCRIBER[" << discovery_guid.c_str() << "]\n";
    }
}

void ParticipantListener::on_publisher_discovery(
        eprosima::fastdds::dds::DomainParticipant* participant,
        eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info)
{
    std::string participant_guid = get_guid(participant);
    std::string discovery_guid = ToString(info.info.guid());

    if (info.status == eprosima::fastrtps::rtps::WriterDiscoveryInfo::DISCOVERED_WRITER)
    {
        std::cout << "New publisher discovered :PARTICIPANT[" << participant_guid.c_str() << "] :DISCOVERY_PUBLISHER[" << discovery_guid.c_str() << "]\n";
    }
    else if (info.status == eprosima::fastrtps::rtps::WriterDiscoveryInfo::REMOVED_WRITER)
    {
        std::cout << "New publisher lost :PARTICIPANT[" << participant_guid.c_str() << "] :DISCOVERY_PUBLISHER[" << discovery_guid.c_str() << "]\n";
    }
}

