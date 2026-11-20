#pragma once

#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>

#define FDDS_DOMAIN_ID "FDDS_DOMAIN_ID"

namespace fdds {

class DDSParticipant {
private:
    DDSParticipant(); 
    ~DDSParticipant();

    DDSParticipant(const DDSParticipant&) = delete;
    DDSParticipant& operator= (const DDSParticipant&) = delete;

private:
    class ParticipantListener : public eprosima::fastdds::dds::DomainParticipantListener {
    public:
        ParticipantListener();
        ~ParticipantListener() override;

        void on_participant_discovery(
                eprosima::fastdds::dds::DomainParticipant* participant,
                eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info) override;

        void on_subscriber_discovery(
                eprosima::fastdds::dds::DomainParticipant* participant,
                eprosima::fastrtps::rtps::ReaderDiscoveryInfo&& info) override;

        void on_publisher_discovery(
                eprosima::fastdds::dds::DomainParticipant* participant,
                eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info) override;
    } listener_;

    eprosima::fastdds::dds::DomainParticipant* participant_ = nullptr;

public:
    eprosima::fastdds::dds::DomainParticipant* getDomainParticipant();
    static DDSParticipant& getInstance();

    // get DomainId of singleton，if setenv FDDS_DOMAIN_ID, use FDDS_DOMAIN_ID, otherwise use 0
    static int getDomainId();
};

}   // fdds
