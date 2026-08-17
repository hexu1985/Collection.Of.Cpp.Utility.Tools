#pragma once

#include "soa_on_dds_types.h"
#include "EprosimaPubWrapper.hpp"
#include "EprosimaSubWrapper.hpp"
#include "worker_thread.hpp"

#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include <string>
#include <memory>

namespace soa_on_dds {

class EprosimaRpcServer {

public:
    EprosimaRpcServer(const std::string& service_name, size_t thread_pool_size, eprosima::fastdds::dds::DomainParticipant* participant=nullptr);

    virtual ~EprosimaRpcServer();

    bool start();

    void stop();

private:
    bool init_request_sub();
    bool init_response_pub();

    class RequestSubListener : public eprosima::fastdds::dds::DataReaderListener {
    public:
        RequestSubListener();

        ~RequestSubListener() override;

        EprosimaRpcServer* m_up;

        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override;
    }
    m_request_sub_listener;

    class ResponsePubListener : public eprosima::fastdds::dds::DataWriterListener {
    public:
        ResponsePubListener();

        ~ResponsePubListener() override;

        EprosimaRpcServer* m_up;
    }
    m_response_pub_listener;

private:
    eprosima::fastdds::dds::DomainParticipant* m_participant=nullptr;
    std::unique_ptr<EprosimaSubWrapper> m_request_sub;
    std::unique_ptr<EprosimaPubWrapper> m_response_pub;
    std::string m_service_name;
    size_t m_thread_pool_size=0;
    std::unique_ptr<worker_thread> m_io_thread;
};

}   // namespace soa_on_dds
