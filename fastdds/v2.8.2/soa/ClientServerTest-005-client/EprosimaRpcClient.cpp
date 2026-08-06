#include "EprosimaRpcClient.hpp"
#include "EprosimaRpcUtils.hpp"
#include "soa_on_dds_typesPubSubTypes.h"

#include <sys/types.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <fastdds/rtps/common/Guid.h>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;

using namespace soa_on_dds;

EprosimaRpcClient::EprosimaRpcClient(const std::string& client_id, const std::string& service_name,
            eprosima::fastdds::dds::DomainParticipant* participant):
    m_client_id(client_id), m_service_name(service_name), m_participant(participant) {

    m_request_pub_listener.m_up = this;
    m_response_sub_listener.m_up = this;
}

EprosimaRpcClient::~EprosimaRpcClient() {
    m_request_pub_listener.m_up = nullptr;
    m_response_sub_listener.m_up = nullptr;

    m_request_pub.reset();
    m_response_sub.reset();
}

bool EprosimaRpcClient::init() {
    if (m_participant == nullptr) {
        return false;
    }

    if (!init_request_pub()) {
        return false;
    }

    if (!init_response_sub()) {
        return false;
    }

    return true;
}

bool EprosimaRpcClient::isReady() {
    if (m_request_pub_matched == 1 && m_response_sub_matched == 1)
    {
        m_is_ready = true;
    }
    else
    {
        m_is_ready = false;
    }
    return m_is_ready;
}

bool EprosimaRpcClient::init_request_pub() {
    m_request_pub.reset(new EprosimaPubWrapper);
    EprosimaPubWrapper::Config config; 
    config.participant = m_participant;
    config.type_support.reset(new soa_on_dds::RPC_RequestPubSubType());
    config.topic_name = EprosimaRpcUtility::generate_rpc_request_topic(m_service_name);
    config.data_writer_listener = &this->m_request_pub_listener;

    DataWriterQos wqos;
    wqos.history().kind = KEEP_LAST_HISTORY_QOS;
    wqos.history().depth = 2;
    wqos.resource_limits().max_samples = 50;
    wqos.resource_limits().allocated_samples = 50;
    config.data_writer_qos = wqos;

    if (!m_request_pub->init(config)) {
        return false;
    }

    return true;
}

bool EprosimaRpcClient::init_response_sub() {
    m_response_sub.reset(new EprosimaSubWrapper);
    EprosimaSubWrapper::Config config;
    config.participant = m_participant;
    config.type_support.reset(new soa_on_dds::RPC_ResponsePubSubType());
    config.topic_name = EprosimaRpcUtility::generate_rpc_response_topic(m_service_name);
    config.data_reader_listener = &this->m_response_sub_listener;

    DataReaderQos rqos;
    rqos.history().kind = KEEP_LAST_HISTORY_QOS;
    rqos.history().depth = 100;
    rqos.resource_limits().max_samples = 100;
    rqos.resource_limits().allocated_samples = 100;
    config.data_reader_qos = rqos;

    if (!m_response_sub->init(config)) {
        return false;
    }

    return true;
}

EprosimaRpcClient::RequestPubListener::RequestPubListener() {
}

EprosimaRpcClient::RequestPubListener::~RequestPubListener() {
}

void EprosimaRpcClient::RequestPubListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter*,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        m_up->m_request_pub_matched++;
    }
    else if (info.current_count_change == -1)
    {
        m_up->m_request_pub_matched--;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
    m_up->isReady();
}

EprosimaRpcClient::ResponseSubListener::ResponseSubListener() {
}

EprosimaRpcClient::ResponseSubListener::~ResponseSubListener() {
}

void EprosimaRpcClient::ResponseSubListener::on_subscription_matched(
        DataReader*,
        const SubscriptionMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        m_up->m_response_sub_matched++;
    }
    else if (info.current_count_change == -1)
    {
        m_up->m_response_sub_matched--;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
    m_up->isReady();
}

void EprosimaRpcClient::ResponseSubListener::on_data_available(
        DataReader*)
{
}
