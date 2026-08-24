#include "EprosimaSubWrapper.hpp"

EprosimaSubWrapper::~EprosimaSubWrapper() {
    reset();
}

bool EprosimaSubWrapper::init(const Config& config) {
    reset();

    m_participant = config.participant;
    if (m_participant == nullptr) {
        goto error_handle;
    }

    //REGISTER TYPES
    m_type_support = config.type_support;
    m_type_support.register_type(m_participant);

    //CREATE THE SUBSCRIBER
    m_subscriber = m_participant->create_subscriber(config.subscriber_qos);
    if (m_subscriber == nullptr) {
        goto error_handle;
    }

    //CREATE THE TOPIC
    m_topic = m_participant->create_topic(config.topic_name, m_type_support.get_type_name(), config.topic_qos);
    if (m_topic == nullptr) {
        goto error_handle;
    }

    //CREATE THE DATAREADER
    m_data_reader = m_subscriber->create_datareader(m_topic, config.data_reader_qos, config.data_reader_listener);
    if (m_data_reader == nullptr) {
        goto error_handle;
    }

    return true;

error_handle:
    reset();
    return false;
}

void EprosimaSubWrapper::reset() {
    if (m_data_reader != nullptr)
    {
        m_subscriber->delete_datareader(m_data_reader);
    }
    if (m_subscriber != nullptr)
    {
        m_participant->delete_subscriber(m_subscriber);
    }
    if (m_topic != nullptr)
    {
        m_participant->delete_topic(m_topic);
    }

    m_data_reader = nullptr;
    m_subscriber = nullptr;
    m_topic = nullptr;
    m_type_support.reset();
    m_participant = nullptr;
}

bool EprosimaSubWrapper::wait_for_unread_message(const eprosima::fastrtps::Duration_t& timeout) {
    if (m_data_reader == nullptr) {
        return false;
    }

    return m_data_reader->wait_for_unread_message(timeout);
}

eprosima::fastrtps::types::ReturnCode_t EprosimaSubWrapper::take_next_sample(void* data, 
        eprosima::fastdds::dds::SampleInfo* info) {
    if (m_data_reader == nullptr) {
        return eprosima::fastrtps::types::ReturnCode_t::RETCODE_ERROR;
    }

    return m_data_reader->take_next_sample(data, info);
}
