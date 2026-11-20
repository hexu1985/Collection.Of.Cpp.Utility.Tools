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

}   // namespace

namespace fdds {

bool DDSPublisherBase::init(const Config& config) {
    return true;
}

void DDSPublisherBase::PubListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    std::string topic_name = get_topic_name(writer);

    if (info.current_count_change == 1)
    {
        matched_ = info.current_count;
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
