#pragma once

#include "soa_on_dds_types.h"
#include "EprosimaPubWrapper.hpp"
#include "EprosimaSubWrapper.hpp"

namespace soa_on_dds {

class EprosimaRpcClient {
public:
    EprosimaRpcClient(const std::string& client_id, const std::string& service_name,
            eprosima::fastdds::dds::DomainParticipant* participant=nullptr);

    virtual ~EprosimaRpcClient();

    bool init();

    bool isReady();

private:
    bool init_request_pub();
    bool init_response_sub();

    class RequestPubListener : public eprosima::fastdds::dds::DataWriterListener {
    public:
        RequestPubListener(); 

        ~RequestPubListener() override;

        EprosimaRpcClient* m_up=nullptr;

        void on_publication_matched(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;
    }
    m_request_pub_listener;

    class ResponseSubListener : public eprosima::fastdds::dds::DataReaderListener {
    public:
        ResponseSubListener(); 

        ~ResponseSubListener() override;

        EprosimaRpcClient* m_up=nullptr;

        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override;

        void on_subscription_matched(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;
    }
    m_response_sub_listener;

    bool m_is_ready=false;

    int m_request_pub_matched=0;

    int m_response_sub_matched=0;

    std::string m_client_id;

    std::string m_service_name;

private:
    eprosima::fastdds::dds::DomainParticipant* m_participant=nullptr;
    std::unique_ptr<EprosimaPubWrapper> m_request_pub;
    std::unique_ptr<EprosimaSubWrapper> m_response_sub;
};

}   // namespace soa_on_dds
