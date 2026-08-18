#include "EprosimaRpcServer.hpp"
#include "EprosimaRpcUtils.hpp"
#include "soa_on_dds_typesPubSubTypes.h"

#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;

using namespace soa_on_dds;

namespace {

constexpr size_t MIN_THREAD_POOL_SIZE=1;
constexpr size_t MAX_THREAD_POOL_SIZE=5;

}   // namespace 

EprosimaRpcServer::EprosimaRpcServer(const std::string& service_name, 
        size_t thread_pool_size, 
        eprosima::fastdds::dds::DomainParticipant* participant): 
    m_participant(participant),
    m_service_name(service_name),
    m_thread_pool_size(thread_pool_size) {
    if (m_thread_pool_size < MIN_THREAD_POOL_SIZE) {
        m_thread_pool_size = MIN_THREAD_POOL_SIZE;
    } 
    if (m_thread_pool_size > MAX_THREAD_POOL_SIZE) {
        m_thread_pool_size = MAX_THREAD_POOL_SIZE;
    }

    m_request_sub_listener.m_up = this;
    m_response_pub_listener.m_up = this;
}

EprosimaRpcServer::~EprosimaRpcServer() {
    stop();
}

bool EprosimaRpcServer::start() {
    if (m_participant == nullptr) {
        return false;
    }

    if (!init_request_sub()) {
        return false;
    }

    if (!init_response_pub()) {
        return false;
    }

    return true;
}

void EprosimaRpcServer::stop() {
    m_main_thread.submit(std::bind(&EprosimaRpcServer::do_stop, this));
}

void EprosimaRpcServer::do_stop() {
    m_request_sub.reset();
    m_response_pub.reset();
}

bool EprosimaRpcServer::init_request_sub() {
    m_request_sub.reset(new EprosimaSubWrapper);
    EprosimaSubWrapper::Config config;
    config.participant = m_participant;
    config.type_support.reset(new soa_on_dds::RPC_RequestPubSubType());
    config.topic_name = EprosimaRpcUtility::generate_rpc_request_topic(m_service_name);
    config.data_reader_listener = &this->m_request_sub_listener;

    DataReaderQos rqos;
    rqos.history().kind = KEEP_LAST_HISTORY_QOS;
    rqos.history().depth = 1000;
    rqos.resource_limits().max_samples = 1500;
    rqos.resource_limits().allocated_samples = 1000;
    config.data_reader_qos = rqos;

    if (!m_request_sub->init(config)) {
        return false;
    }

    return true;
}

bool EprosimaRpcServer::init_response_pub() {
    m_response_pub.reset(new EprosimaPubWrapper);
    EprosimaPubWrapper::Config config; 
    config.participant = m_participant;
    config.type_support.reset(new soa_on_dds::RPC_ResponsePubSubType());
    config.topic_name = EprosimaRpcUtility::generate_rpc_response_topic(m_service_name);
    config.data_writer_listener = &this->m_response_pub_listener;

    DataWriterQos wqos;
    wqos.history().kind = KEEP_LAST_HISTORY_QOS;
    wqos.history().depth = 1000;
    wqos.resource_limits().max_samples = 1500;
    wqos.resource_limits().allocated_samples = 1000;
    wqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    config.data_writer_qos = wqos;

    if (!m_response_pub->init(config)) {
        return false;
    }

    return true;
}

void EprosimaRpcServer::register_method_helper(const std::string& method_name, IMethodHandlerPtr method_handler) {
    m_main_thread.submit(std::bind(&EprosimaRpcServer::do_register_method, this, method_name, method_handler));
}

void EprosimaRpcServer::do_register_method(const std::string& method_name, IMethodHandlerPtr method_handler) {
    m_method_handlers[method_name]= method_handler;
}

void EprosimaRpcServer::unregister_method(const std::string& method_name) {
    m_main_thread.submit(std::bind(&EprosimaRpcServer::do_unregister_method, this, method_name));
}

void EprosimaRpcServer::do_unregister_method(const std::string& method_name) {
    m_method_handlers.erase(method_name);
}

void EprosimaRpcServer::on_data_available() {
    m_main_thread.submit(std::bind(&EprosimaRpcServer::do_recv_request, this));
}

void EprosimaRpcServer::do_recv_request() {
    std::cout << "EprosimaRpcClient::do_recv_response" << std::endl;
    int count = 0;
    auto rpc_request = std::make_shared<soa_on_dds::RPC_Request>();
    while (m_request_sub->take_next_sample((void*) rpc_request.get(), &m_sample_info) == ReturnCode_t::RETCODE_OK ) {
        if (m_sample_info.instance_state == eprosima::fastdds::dds::ALIVE_INSTANCE_STATE) {

        }
        count++;
        if (count > MAX_THREAD_POOL_SIZE) {
        }
    }
}

EprosimaRpcServer::IMethodHandlerPtr EprosimaRpcServer::get_method_handler(const std::string& method_name) {
    auto iter = m_method_handlers.find(method_name);
    if (iter == m_method_handlers.end()) {
        return nullptr;
    }

    return iter->second;
}

EprosimaRpcServer::RequestSubListener::RequestSubListener() {
}

EprosimaRpcServer::RequestSubListener::~RequestSubListener() {
}

void EprosimaRpcServer::RequestSubListener::on_data_available(
        eprosima::fastdds::dds::DataReader* reader) {
}

EprosimaRpcServer::ResponsePubListener::ResponsePubListener() {
}

EprosimaRpcServer::ResponsePubListener::~ResponsePubListener() {
}

EprosimaRpcServer::IMethodHandler::IMethodHandler() {
}

EprosimaRpcServer::IMethodHandler::~IMethodHandler() {
}
