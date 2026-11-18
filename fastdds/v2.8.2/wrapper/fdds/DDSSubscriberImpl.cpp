#include "DDSSubscriberImpl.hpp"

namespace fdds {

DDSSubscriberImpl::~DDSSubscriberImpl() {
    reset();
}

bool DDSSubscriberImpl::init(const Config& config) {
    reset();

    participant_ = config.participant;
    if (participant_ == nullptr) {
        goto error_handle;
    }

    //REGISTER TYPES
    type_ = config.type_support;
    type_.register_type(participant_);

    //CREATE THE SUBSCRIBER
    subscriber_ = participant_->create_subscriber(config.subscriber_qos);
    if (subscriber_ == nullptr) {
        goto error_handle;
    }

    //CREATE THE TOPIC
    topic_ = participant_->create_topic(config.topic_name, type_.get_type_name(), config.topic_qos);
    if (topic_ == nullptr) {
        goto error_handle;
    }

    //CREATE THE DATAREADER
    reader_ = subscriber_->create_datareader(topic_, config.data_reader_qos, config.data_reader_listener);
    if (reader_ == nullptr) {
        goto error_handle;
    }

    return true;

error_handle:
    reset();
    return false;
}

void DDSSubscriberImpl::reset() {
    if (reader_ != nullptr)
    {
        subscriber_->delete_datareader(reader_);
    }
    if (subscriber_ != nullptr)
    {
        participant_->delete_subscriber(subscriber_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }

    reader_ = nullptr;
    subscriber_ = nullptr;
    topic_ = nullptr;
    type_.reset();
    participant_ = nullptr;
}

eprosima::fastrtps::types::ReturnCode_t DDSSubscriberImpl::take_next_sample(void* data, 
        eprosima::fastdds::dds::SampleInfo* info) {
    if (reader_ == nullptr) {
        return eprosima::fastrtps::types::ReturnCode_t::RETCODE_ERROR;
    }

    return reader_->take_next_sample(data, info);
}

}   // namespace fdds

