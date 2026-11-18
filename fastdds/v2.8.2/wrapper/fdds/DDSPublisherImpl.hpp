#pragma once

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>

namespace fdds {

class DDSPublisherImpl {
public:
    struct Config {
        eprosima::fastdds::dds::DomainParticipant* participant=nullptr;
        eprosima::fastdds::dds::TypeSupport type_support;
        eprosima::fastdds::dds::PublisherQos publisher_qos=eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT;
        eprosima::fastdds::dds::TopicQos topic_qos=eprosima::fastdds::dds::TOPIC_QOS_DEFAULT;
        std::string topic_name;
        eprosima::fastdds::dds::DataWriterQos data_writer_qos=eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT;
        eprosima::fastdds::dds::DataWriterListener* data_writer_listener=nullptr;
    };

    DDSPublisherImpl() = default;
    
    virtual ~DDSPublisherImpl();

    DDSPublisherImpl(const DDSPublisherImpl&) = delete;
    const DDSPublisherImpl& operator =(const DDSPublisherImpl&) = delete;

    bool init(const Config& config);

    bool write(void* data);

private:
    void reset();

private:
    eprosima::fastdds::dds::DomainParticipant* m_participant=nullptr;

    eprosima::fastdds::dds::TypeSupport m_type_support;

    eprosima::fastdds::dds::Publisher* m_publisher=nullptr;

    eprosima::fastdds::dds::Topic* m_topic=nullptr;

    eprosima::fastdds::dds::DataWriter* m_data_writer=nullptr;
};

}   // namespace fdds
