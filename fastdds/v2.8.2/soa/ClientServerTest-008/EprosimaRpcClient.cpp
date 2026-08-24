#include "EprosimaRpcClient.hpp"
#include "EprosimaRpcUtility.hpp"
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

EprosimaRpcClient::EprosimaRpcClient(const std::string& client_name, const std::string& service_name,
            eprosima::fastdds::dds::DomainParticipant* participant):
    m_service_name(service_name), m_participant(participant) {
    //std::cout << "EprosimaRpcClient::EprosimaRpcClient" << std::endl;

    if (m_participant == nullptr) {
        m_participant = EprosimaRpcUtility::get_default_rpc_participant();
    }

    m_client_id = EprosimaRpcUtility::generate_rpc_client_id(client_name);
    //std::cout << "m_client_id: " << m_client_id << std::endl;

    m_session_id = EprosimaRpcUtility::generate_rpc_session_id();
    //std::cout << "m_session_id: " << m_session_id << std::endl;

    m_request_pub_listener.m_up = this;
    m_response_sub_listener.m_up = this;

    m_cached_response = std::make_shared<soa_on_dds::RPC_Response>();
}

EprosimaRpcClient::~EprosimaRpcClient() {
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

bool EprosimaRpcClient::is_ready() {
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

long EprosimaRpcClient::send_request(const std::string& method_name, const std::vector<uint8_t>& request_payload,
        ResponsePromisePtr response_promise) {
    auto request = make_rpc_request(method_name, request_payload);
    auto request_info = std::make_shared<RequestInfo>();
    request_info->request = request;
    request_info->response_promise = response_promise;

    m_worker.submit(std::bind(&EprosimaRpcClient::do_send_request, this, request_info));

    return request->header().request_id();
}

void EprosimaRpcClient::do_send_request(RequestInfoPtr request_info) {
    auto request = request_info->request;
    auto response_promise = request_info->response_promise;
    if (!m_request_pub->write((void*)request.get())) {
        auto response = make_rpc_response(request, soa_on_dds::CLIENT_SEND_REQUEST_ERROR);
        response_promise->set_value(response);
        std::cout << "m_request_pub->write failed" << std::endl;
        return;
    }

    //std::cout << "insert request[" << request->header().request_id() << "]" << std::endl;
    m_pending_requests[request->header().request_id()] = request_info;
    return;
}

void EprosimaRpcClient::on_data_available() {
    m_worker.submit(std::bind(&EprosimaRpcClient::do_recv_response, this));
}

void EprosimaRpcClient::remove_pending_request(long request_id) {
    m_worker.submit(std::bind(&EprosimaRpcClient::do_remove_pending_request, this, request_id));
}

void EprosimaRpcClient::do_remove_pending_request(long request_id) {
    m_pending_requests.erase(request_id);
}

void EprosimaRpcClient::do_recv_response() {
    //std::cout << "EprosimaRpcClient::do_recv_response" << std::endl;
    while (m_response_sub->take_next_sample((void*) m_cached_response.get(), &m_sample_info) == ReturnCode_t::RETCODE_OK ) {
        if (m_sample_info.instance_state != eprosima::fastdds::dds::ALIVE_INSTANCE_STATE) {
            continue;
        }
        if (is_valid_response(m_cached_response)) {
            auto rpc_response = m_cached_response;
            m_cached_response = std::make_shared<soa_on_dds::RPC_Response>();
            dispatch_response(rpc_response);
        }
    }
}

bool EprosimaRpcClient::is_valid_response(ResponsePtr rpc_response) {
    if (rpc_response->header().client_id() != m_client_id) {
        std::cout << "client id mismatched: m_client_id[" << m_client_id << "], response.client_id[" << rpc_response->header().client_id() << "]" << std::endl;
        return false;
    }

    if (rpc_response->header().session_id() != m_session_id) {
        std::cout << "session id mismatched: m_session_id[" << m_session_id << "], response.session_id[" << rpc_response->header().session_id() << "]" << std::endl;
        return false;
    }

    return true;
}

void EprosimaRpcClient::dispatch_response(std::shared_ptr<RPC_Response> rpc_response) {
    long request_id = rpc_response->header().request_id();
    auto request_info = get_request_info(request_id);
    if (request_info == nullptr) {
        std::cout << "Not found request_info by request_id[" << request_id << "]" << std::endl;
        return;
    }

    // check method name
    auto& request_method_name = request_info->request->header().method_name();
    auto& response_method_name = rpc_response->header().method_name();
    if (request_method_name != response_method_name) {
        std::cout << "Warning: request_method_name[" << request_method_name << "] != response_method_name["
                  << response_method_name << "]" << std::endl;
        return;
    }

    request_info->response_promise->set_value(rpc_response);
}

EprosimaRpcClient::RequestPtr EprosimaRpcClient::make_rpc_request(
        const std::string& method_name, const std::vector<uint8_t>& request_payload) {
    auto rpc_request = std::make_shared<soa_on_dds::RPC_Request>();
    rpc_request->header().method_name(method_name);
    rpc_request->header().client_id(m_client_id);
    rpc_request->header().session_id(m_session_id);
    long request_id = ++m_request_id_generator;
    rpc_request->header().request_id(request_id);
    rpc_request->request_payload(request_payload);

    return rpc_request;
}

EprosimaRpcClient::ResponsePtr EprosimaRpcClient::make_rpc_response(RequestPtr rpc_request, soa_on_dds::ErrorCode error_code) {
    auto rpc_response = std::make_shared<soa_on_dds::RPC_Response>();
    rpc_response->header(rpc_request->header());
    rpc_response->error_code(error_code);
    return rpc_response;
}

EprosimaRpcClient::RequestInfoPtr EprosimaRpcClient::get_request_info(long request_id) {
    auto iter = m_pending_requests.find(request_id);
    if (iter == m_pending_requests.end()) {
        return nullptr;
    }

    return iter->second;
}

EprosimaRpcClient::RequestPubListener::RequestPubListener() {
}

EprosimaRpcClient::RequestPubListener::~RequestPubListener() {
}

void EprosimaRpcClient::RequestPubListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter*,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    if (m_up == nullptr) {
        std::cout << "m_up is nullptr" << std::endl;
        return;
    }

    if (info.current_count_change == 1)
    {
        std::cout << "EprosimaRpcClient::RequestPubListener matched" << std::endl;
        m_up->m_request_pub_matched++;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "EprosimaRpcClient::RequestPubListener unmatched" << std::endl;
        m_up->m_request_pub_matched--;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
    m_up->is_ready();
}

EprosimaRpcClient::ResponseSubListener::ResponseSubListener() {
}

EprosimaRpcClient::ResponseSubListener::~ResponseSubListener() {
}

void EprosimaRpcClient::ResponseSubListener::on_subscription_matched(
        DataReader*,
        const SubscriptionMatchedStatus& info)
{
    if (m_up == nullptr) {
        std::cout << "m_up is nullptr" << std::endl;
        return;
    }

    if (info.current_count_change == 1)
    {
        std::cout << "EprosimaRpcClient::ResponseSubListener matched" << std::endl;
        m_up->m_response_sub_matched++;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "EprosimaRpcClient::ResponseSubListener unmatched" << std::endl;
        m_up->m_response_sub_matched--;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
    m_up->is_ready();
}

void EprosimaRpcClient::ResponseSubListener::on_data_available(
        DataReader*)
{
    if (m_up == nullptr) {
        std::cout << "m_up is nullptr" << std::endl;
        return;
    }

    m_up->on_data_available();
}
