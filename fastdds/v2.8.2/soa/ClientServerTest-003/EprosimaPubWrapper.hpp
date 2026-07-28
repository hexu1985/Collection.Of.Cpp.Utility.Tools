#pragma once

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>

class EprosimaPubWrapper {
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

    EprosimaPubWrapper() = default;
    
    virtual ~EprosimaPubWrapper();

    bool init(const Config& config);

private:
    void reset();

private:
    eprosima::fastdds::dds::DomainParticipant* mp_participant=nullptr;

    eprosima::fastdds::dds::TypeSupport mp_type_support;

    eprosima::fastdds::dds::Publisher* mp_publisher=nullptr;

    eprosima::fastdds::dds::Topic* mp_topic=nullptr;

    eprosima::fastdds::dds::DataWriter* mp_data_writer=nullptr;
};
