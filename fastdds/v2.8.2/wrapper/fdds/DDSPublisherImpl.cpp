#include "DDSPublisherImpl.hpp"

namespace fdds {

DDSPublisherImpl::~DDSPublisherImpl() {
    reset();
}

bool DDSPublisherImpl::init(const Config& config) {
    reset();

    participant_ = config.participant;
    if (participant_ == nullptr) {
        goto error_handle;
    }

    //REGISTER TYPES
    type_ = config.type_support;
    type_.register_type(participant_);

    //CREATE THE PUBLISHER
    publisher_ = participant_->create_publisher(config.publisher_qos);
    if (publisher_ == nullptr) {
        goto error_handle;
    }

    //CREATE THE TOPIC
    topic_ = participant_->create_topic(config.topic_name, type_.get_type_name(), config.topic_qos);
    if (topic_ == nullptr) {
        goto error_handle;
    }

    //CREATE THE DATAWRITER
    writer_ = publisher_->create_datawriter(topic_, config.data_writer_qos, config.data_writer_listener);
    if (writer_ == nullptr) {
        goto error_handle;
    }

    return true;

error_handle:
    reset();
    return false;
}

void DDSPublisherImpl::reset() {
    if (writer_ != nullptr)
    {
        publisher_->delete_datawriter(writer_);
    }
    if (publisher_ != nullptr)
    {
        participant_->delete_publisher(publisher_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }

    writer_ = nullptr;
    publisher_ = nullptr;
    topic_ = nullptr;
    type_.reset();
    participant_ = nullptr;
}

bool DDSPublisherImpl::write(void* data) {
    if (writer_ == nullptr) {
        return false;
    }

    return writer_->write(data);
}

}   // namespace fdds

