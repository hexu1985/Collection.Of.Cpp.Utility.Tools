#pragma once

#include "DDSPublisherImpl.hpp"
#include <fastdds/dds/publisher/DataWriterListener.hpp>

namespace fdds {

class DDSPublisherBase {
public:
    DDSPublisherBase(); 

    bool init();

protected:
    class PubListener : public eprosima::fastdds::dds::DataWriterListener {
    public:
        PubListener(): matched_(0), firstConnected_(false) {
        }

        ~PubListener() override {
        }

        void on_publication_matched(
                eprosima::fastdds::dds::DataWriter* DataWriter,
                const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

        void on_offered_deadline_missed(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::OfferedDeadlineMissedStatus& status) override;

        void on_offered_incompatible_qos(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::OfferedIncompatibleQosStatus& status) override;

        void on_liveliness_lost(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::LivelinessLostStatus& status) override;

        int matched_;
        bool firstConnected_;
    }
    listener_;
    DDSPublisherImpl impl_;
};

}   // namespace fdds
