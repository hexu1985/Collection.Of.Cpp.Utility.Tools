#include "DDSPublisherBase.hpp"

#include <string>

namespace {

std::string get_topic_name(eprosima::fastdds::dds::DataWriter* writer) {
    if (writer == nullptr) {
        return "";
    }

    eprosima::fastdds::dds::Topic* topic = writer->get_topic();
    if (topic == nullptr) {
        return "";
    }

    return topic->get_name();
}

fdds::DDSPublisherImpl::Config make_publisher_impl_config(
        const fdds::DDSPublisherBase::Config& base_config,
        eprosima::fastdds::dds::DataWriterListener* listener
        ) {
    fdds::DDSPublisherImpl::Config impl_config;

    // participant
    impl_config.participant = base_config.participant;

    // type and topic name
    impl_config.type_support = base_config.type_support;
    if (base_config.topic_name.empty()) {
        impl_config.topic_name = base_config.type_support.get_type_name();
    } else {
        impl_config.topic_name = base_config.topic_name;
    }

    // reliability
    impl_config.data_writer_qos.reliability().kind = base_config.reliability_kind;
    impl_config.data_writer_qos.reliability().max_blocking_time = base_config.max_blocking_time;

    // history
    impl_config.data_writer_qos.history().kind = base_config.history_kind;
    impl_config.data_writer_qos.history().depth = base_config.history_depth;

    // resource limits
    impl_config.data_writer_qos.resource_limits().max_samples = base_config.max_samples;
    impl_config.data_writer_qos.resource_limits().max_instances = base_config.max_instances;
    impl_config.data_writer_qos.resource_limits().max_samples_per_instance = base_config.max_samples_per_instance;

    // publish mode
    impl_config.data_writer_qos.publish_mode().kind = base_config.publish_mode_kind;

    // listener
    impl_config.data_writer_listener = listener;

    return impl_config;
}  

}   // namespace

namespace fdds {

bool DDSPublisherBase::init(const Config& config) {
    auto impl_config = make_publisher_impl_config(config, &listener_);
    return impl_.init(impl_config);
}

bool DDSPublisherBase::publish(void* data) {
    if (listener_.firstConnected_ || listener_.matched_ > 0)
    {
        return impl_.publish(data);
    } else {
        logError(DDS_PUBLISHER_BASE, "publish failed: no subscriber matched");
        return false;
    }
}

void DDSPublisherBase::PubListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    std::string topic_name = get_topic_name(writer);

    if (info.current_count_change == 1)
    {
        matched_ = info.current_count;
        firstConnected_ = true;
        logWarning(DDS_PUBLISHER_BASE, "Publisher matched. matched_: " << matched_ << ":TOPIC[" << topic_name << "]");
    }
    else if (info.current_count_change == -1)
    {
        matched_ = info.current_count;
        logWarning(DDS_PUBLISHER_BASE, "Publisher unmatched. matched_: " << matched_ << ":TOPIC[" << topic_name << "]");
    }
    else
    {
        logWarning(DDS_PUBLISHER_BASE, info.current_count_change << " is not a valid value for PublicationMatchedStatus current count change." << ":TOPIC[" << topic_name << "]");
    }
}

void DDSPublisherBase::PubListener::on_offered_deadline_missed(
        eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::OfferedDeadlineMissedStatus& status)
{
    std::string topic_name = get_topic_name(writer);

    (void)status;
    logWarning(DDS_PUBLISHER_BASE, "Some data could not be delivered on time." << ":TOPIC[" << topic_name << "]");
}

void DDSPublisherBase::PubListener::on_offered_incompatible_qos(
        eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::OfferedIncompatibleQosStatus& status)
{
    std::string topic_name = get_topic_name(writer);

    (void)status;
    logWarning(DDS_PUBLISHER_BASE, "Found a remote Topic with incompatible QoS (QoS ID: " << status.last_policy_id << ":TOPIC[" << topic_name << "]");
}

void DDSPublisherBase::PubListener::on_liveliness_lost(
        eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::LivelinessLostStatus& status)
{
    std::string topic_name = get_topic_name(writer);

    (void)status;
    logWarning(DDS_PUBLISHER_BASE, "Liveliness lost. Matched Subscribers will consider us offline." << ":TOPIC[" << topic_name << "]");
}

}   // namespace fdds
