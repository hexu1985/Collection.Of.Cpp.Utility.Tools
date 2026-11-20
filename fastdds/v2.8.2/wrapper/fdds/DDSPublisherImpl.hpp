#pragma once

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>

namespace fdds {

class DDSPublisherImpl {
public:
    using DomainParticipant = eprosima::fastdds::dds::DomainParticipant;
    using TypeSupport = eprosima::fastdds::dds::TypeSupport;
    using PublisherQos = eprosima::fastdds::dds::PublisherQos;
    using TopicQos = eprosima::fastdds::dds::TopicQos;
    using DataWriterQos = eprosima::fastdds::dds::DataWriterQos;
    using DataWriterListener = eprosima::fastdds::dds::DataWriterListener;

    struct Config {
        DomainParticipant* participant = nullptr;
        TypeSupport type_support;
        PublisherQos publisher_qos = eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT;
        TopicQos topic_qos = eprosima::fastdds::dds::TOPIC_QOS_DEFAULT;
        std::string topic_name;
        DataWriterQos data_writer_qos=eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT;
        DataWriterListener* data_writer_listener = nullptr;
    };

    DDSPublisherImpl() = default;
    
    virtual ~DDSPublisherImpl();

    DDSPublisherImpl(const DDSPublisherImpl&) = delete;
    const DDSPublisherImpl& operator =(const DDSPublisherImpl&) = delete;

    bool init(const Config& config);

    bool publish(void* data);

private:
    void reset();

private:
    eprosima::fastdds::dds::DomainParticipant* participant_ = nullptr;

    eprosima::fastdds::dds::TypeSupport type_;

    eprosima::fastdds::dds::Publisher* publisher_ = nullptr;

    eprosima::fastdds::dds::Topic* topic_ = nullptr;

    eprosima::fastdds::dds::DataWriter* writer_ = nullptr;
};

}   // namespace fdds
