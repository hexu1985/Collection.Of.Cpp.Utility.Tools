#pragma once

#include <fastdds/dds/domain/DomainParticipantListener.hpp>

class ParticipantListener : public eprosima::fastdds::dds::DomainParticipantListener
{
public:

    ParticipantListener()
        : DomainParticipantListener()
    {
    }

    virtual ~ParticipantListener() override
    {
    }

    virtual void on_participant_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info) override;

    virtual void on_subscriber_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::ReaderDiscoveryInfo&& info) override;

    virtual void on_publisher_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info) override;
};

