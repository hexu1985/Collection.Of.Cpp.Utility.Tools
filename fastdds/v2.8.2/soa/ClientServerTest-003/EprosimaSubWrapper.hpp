#pragma once

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>

class EprosimaSubWrapper {
public:
    struct Config {
        eprosima::fastdds::dds::DomainParticipant* participant=nullptr;
        eprosima::fastdds::dds::TypeSupport type_support;
        eprosima::fastdds::dds::SubscriberQos subscriber_qos=eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT;
        eprosima::fastdds::dds::TopicQos topic_qos=eprosima::fastdds::dds::TOPIC_QOS_DEFAULT;
        std::string topic_name;
        eprosima::fastdds::dds::DataReaderQos data_reader_qos=eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT;
        eprosima::fastdds::dds::DataReaderListener* data_reader_listener=nullptr;
    };

    EprosimaSubWrapper() = default;
    
    virtual ~EprosimaSubWrapper();

    EprosimaSubWrapper(const EprosimaSubWrapper&) = delete;
    const EprosimaSubWrapper& operator =(const EprosimaSubWrapper&) = delete;

    bool init(const Config& config);

private:
    void reset();

private:
    eprosima::fastdds::dds::DomainParticipant* m_participant=nullptr;

    eprosima::fastdds::dds::TypeSupport m_type_support;

    eprosima::fastdds::dds::Subscriber* m_subscriber=nullptr;

    eprosima::fastdds::dds::Topic* m_topic=nullptr;

    eprosima::fastdds::dds::DataReader* m_data_reader=nullptr;
};

