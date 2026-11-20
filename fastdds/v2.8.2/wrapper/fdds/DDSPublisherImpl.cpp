#include "DDSPublisherImpl.hpp"
#include "DDSUtility.hpp"

using namespace eprosima::fastdds::dds;

namespace fdds {

DDSPublisherImpl::~DDSPublisherImpl() {
    reset();
}

bool DDSPublisherImpl::init(const Config& config) {
    reset();

    participant_ = config.participant;
    if (participant_ == nullptr) {
        logError(DDS_PUBLISHER_IMPL, "config.participant is nullptr");
        return false;
    }

    //REGISTER TYPES
    type_ = config.type_support;
    type_.register_type(participant_);
    std::string prefix_profile_name = type_.get_type_name() + "_" + config.topic_name;
    logWarning(DDS_PUBLISHER_IMPL, "prefix_profile_name: " << prefix_profile_name);

    //CREATE THE PUBLISHER
    PublisherQos pubqos = config.publisher_qos;
    if(ReturnCode_t::RETCODE_OK == participant_->get_publisher_qos_from_profile(prefix_profile_name + "_pub_profile", pubqos))
    {
        logWarning(DDS_PUBLISHER_IMPL, "get_publisher_qos_from_profile: using " << prefix_profile_name + "_pub_profile");
    } else if (ReturnCode_t::RETCODE_OK == participant_->get_publisher_qos_from_profile("publisher_profile_default", pubqos)) {
        logWarning(DDS_PUBLISHER_IMPL, "get_publisher_qos_from_profile: using publisher_profile_default");
    } else {
        logWarning(DDS_PUBLISHER_IMPL, "using config.publisher_qos");
    }
    publisher_ = participant_->create_publisher(pubqos);
    if (publisher_ == nullptr) {
        logError(DDS_PUBLISHER_IMPL, "create_publisher failed");
        return false;
    }

    //CREATE THE TOPIC
    TopicQos tqos = config.topic_qos;
    topic_ = participant_->create_topic(config.topic_name, type_.get_type_name(), tqos);
    if (topic_ == nullptr) {
        logError(DDS_PUBLISHER_IMPL, "create_topic failed");
        return false;
    }

    //CREATE THE DATAWRITER
    DataWriterQos wqos = config.data_writer_qos;
    if (ReturnCode_t::RETCODE_OK == publisher_->get_datawriter_qos_from_profile(prefix_profile_name + "_datawriter_profile", wqos)) {
        logWarning(DDS_PUBLISHER_IMPL, "get_datawriter_qos_from_profile: using " << prefix_profile_name + "_datawriter_profile");
    } else if (wqos.reliability().kind == BEST_EFFORT_RELIABILITY_QOS && ReturnCode_t::RETCODE_OK == publisher_->get_datawriter_qos_from_profile("best_effort_datawriter_profile_default", wqos)) {
        logWarning(DDS_PUBLISHER_IMPL, "get_datawriter_qos_from_profile: using best_effort_datawriter_profile_default");
    } else if (wqos.reliability().kind == RELIABLE_RELIABILITY_QOS && ReturnCode_t::RETCODE_OK == publisher_->get_datawriter_qos_from_profile("reliable_datawriter_profile_default", wqos)) {
        logWarning(DDS_PUBLISHER_IMPL, "get_datawriter_qos_from_profile: using reliable_datawriter_profile_default");
    } else {
        logWarning(DDS_PUBLISHER_IMPL, "using config.data_writer_qos");
    }
    writer_ = publisher_->create_datawriter(topic_, wqos, config.data_writer_listener);
    if (writer_ == nullptr) {
        logError(DDS_PUBLISHER_IMPL, "create_datawriter failed");
        return false;
    }

    logWarning(DDS_PUBLISHER_IMPL, config.data_writer_qos);

    return true;
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
    //type_.reset();
    participant_ = nullptr;
}

bool DDSPublisherImpl::publish(void* data) {
    if (writer_ == nullptr) {
        logError(DDS_PUBLISHER_IMPL, "writer_ is nullptr");
        return false;
    }

    return writer_->write(data);
}

}   // namespace fdds

