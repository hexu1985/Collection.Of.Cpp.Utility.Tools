#include "EprosimaPubWrapper.hpp"

EprosimaPubWrapper::~EprosimaPubWrapper() {
    reset();
}

bool EprosimaPubWrapper::init(const Config& config) {
    reset();

    m_participant = config.participant;
    if (m_participant == nullptr) {
        goto error_handle;
    }

    //REGISTER TYPES
    m_type_support = config.type_support;
    m_type_support.register_type(m_participant);

    //CREATE THE PUBLISHER
    m_publisher = m_participant->create_publisher(config.publisher_qos);
    if (m_publisher == nullptr) {
        goto error_handle;
    }

    //CREATE THE TOPIC
    m_topic = m_participant->create_topic(config.topic_name, m_type_support.get_type_name(), config.topic_qos);
    if (m_topic == nullptr) {
        goto error_handle;
    }

    //CREATE THE DATAWRITER
    m_data_writer = m_publisher->create_datawriter(m_topic, config.data_writer_qos, config.data_writer_listener);
    if (m_data_writer == nullptr) {
        goto error_handle;
    }

    return true;

error_handle:
    reset();
    return false;
}

void EprosimaPubWrapper::reset() {
    if (m_data_writer != nullptr)
    {
        m_publisher->delete_datawriter(m_data_writer);
    }
    if (m_publisher != nullptr)
    {
        m_participant->delete_publisher(m_publisher);
    }
    if (m_topic != nullptr)
    {
        m_participant->delete_topic(m_topic);
    }

    m_data_writer = nullptr;
    m_publisher = nullptr;
    m_topic = nullptr;
    m_type_support.reset();
    m_participant = nullptr;
}

bool EprosimaPubWrapper::write(void* data) {
    if (m_data_writer == nullptr) {
        return false;
    }

    return m_data_writer->write(data);
}

