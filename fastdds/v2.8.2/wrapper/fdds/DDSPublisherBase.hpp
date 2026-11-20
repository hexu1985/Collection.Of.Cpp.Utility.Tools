#pragma once

#include "DDSPublisherImpl.hpp"
#include <fastdds/dds/publisher/DataWriterListener.hpp>

namespace fdds {

class DDSPublisherBase {
public:
    using HistoryQosPolicyKind = eprosima::fastdds::dds::HistoryQosPolicyKind;
    using ReliabilityQosPolicyKind = eprosima::fastdds::dds::ReliabilityQosPolicyKind;
    using Duration_t = eprosima::fastrtps::Duration_t;
    using PublishModeQosPolicyKind = eprosima::fastdds::dds::PublishModeQosPolicyKind;
    using ResourceLimitsQosPolicy = eprosima::fastdds::dds::ResourceLimitsQosPolicy;

    struct Config {
        eprosima::fastdds::dds::TypeSupport type_support;
        std::string topic_name;
        ReliabilityQosPolicyKind reliability_kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
        Duration_t max_blocking_time = Duration_t{0, 100000000}; // max_blocking_time = 100ms
        HistoryQosPolicyKind history_kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
        int history_depth = 1;
        int max_samples = 5000;
        int max_instances = 10;
        int max_samples_per_instance = 400;
        PublishModeQosPolicyKind publish_mode_kind = eprosima::fastdds::dds::SYNCHRONOUS_PUBLISH_MODE;
    };

    DDSPublisherBase(); 

    bool init(const Config& config);

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
