// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file EprosimaClient.cpp
 *
 */

#include "EprosimaClient.h"

#include <sys/types.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <fastdds/rtps/common/Guid.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include "soa_on_dds_typesPubSubTypes.h"
#include "EprosimaParticipantManager.hpp"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;
using namespace clientserver;

namespace {

std::string guid_to_string(const eprosima::fastrtps::rtps::GUID_t& guid) {
    std::stringstream ss;
    // GuidPrefix_t 已经支持通过 operator<< 输出为 "xx.xx.xx..." 格式
    ss << guid.guidPrefix;
    // 需要手动拼接 EntityId 部分，这里以十六进制输出，例如 "|0.0.0.1"
    ss << "|"
       << std::hex << std::setfill('0') << std::setw(2)
       << (int)guid.entityId.value[0] << "."
       << std::setw(2) << (int)guid.entityId.value[1] << "."
       << std::setw(2) << (int)guid.entityId.value[2] << "."
       << std::setw(2) << (int)guid.entityId.value[3];
    return ss.str();
}

}   // namespace

EprosimaClient::EprosimaClient()
    : mp_participant(nullptr)
    , m_operationsListener(nullptr)
    , m_resultsListener(nullptr)
    , m_isReady(false)
    , m_operationMatched(0)
    , m_resultMatched(0)
{
    m_operationsListener.mp_up = this;
    m_resultsListener.mp_up = this;
}

EprosimaClient::~EprosimaClient()
{
    mp_operation_pub.reset();
    mp_result_sub.reset();
    EprosimaParticipantManager::delete_participant(mp_participant);
}

bool EprosimaClient::init()
{
    if (!init_participant()) {
        return false;
    }

    if (!init_operation_pub()) {
        return false;
    }

    if (!init_result_sub()) {
        return false;
    }

    return true;
}

bool EprosimaClient::init_participant() {
    //CREATE THE PARTICIPANT
    DomainParticipantQos pqos;
    pqos.wire_protocol().builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol =
            eprosima::fastrtps::rtps::DiscoveryProtocol::SIMPLE;
    pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
    pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.name( "client_RTPSParticipant");

    mp_participant = EprosimaParticipantManager::create_participant(0, pqos);

    if (mp_participant == nullptr)
    {
        return false;
    }

    return true;
}

bool EprosimaClient::init_operation_pub() {
    mp_operation_pub.reset(new EprosimaPubWrapper);
    EprosimaPubWrapper::Config config; 
    config.participant = mp_participant;
    config.type_support.reset(new soa_on_dds::RPC_RequestPubSubType());
    config.topic_name = "soa.rpc.compute.request";
    config.data_writer_listener = &this->m_operationsListener;

    DataWriterQos wqos;
    wqos.history().kind = KEEP_LAST_HISTORY_QOS;
    wqos.history().depth = 2;
    wqos.resource_limits().max_samples = 50;
    wqos.resource_limits().allocated_samples = 50;
    config.data_writer_qos = wqos;

    if (!mp_operation_pub->init(config)) {
        return false;
    }

    return true;
}

bool EprosimaClient::init_result_sub() {
    mp_result_sub.reset(new EprosimaSubWrapper);
    EprosimaSubWrapper::Config config;
    config.participant = mp_participant;
    config.type_support.reset(new soa_on_dds::RPC_ResponsePubSubType());
    config.topic_name = "soa.rpc.compute.response";
    config.data_reader_listener = &this->m_resultsListener;

    DataReaderQos rqos;
    rqos.history().kind = KEEP_LAST_HISTORY_QOS;
    rqos.history().depth = 100;
    rqos.resource_limits().max_samples = 100;
    rqos.resource_limits().allocated_samples = 100;
    config.data_reader_qos = rqos;

    if (!mp_result_sub->init(config)) {
        return false;
    }

    return true;
}

soa_on_dds::ErrorCode EprosimaClient::calculate(
        Operation::OPERATIONTYPE type,
        int32_t num1,
        int32_t num2,
        int32_t* result)
{
    SampleInfo m_sampleInfo;
    if (!m_isReady)
    {
        return soa_on_dds::SERVICE_NOT_AVAILABLE;
    }
    clientserver::Operation operation;
    operation.m_operationType = type;
    operation.m_num1 = num1;
    operation.m_num2 = num2;

    m_rpc_request.header().method_name("operation");
    m_rpc_request.header().client_id("client");
    m_rpc_request.header().session_id(getpid());
    m_rpc_request.header().request_id(
        m_rpc_request.header().request_id()+1
    );

    std::vector<uint8_t> request_payload;
    if (!operation.SerializeToVector(request_payload)) {
        std::cout << "SerializeToVector failed" << std::endl;
        return soa_on_dds::SERIALIZE_FAILED;
    }
    std::cout << "request_payload.size(): " << request_payload.size() << std::endl;
    m_rpc_request.request_payload(request_payload);

    mp_operation_pub->write((void*)&m_rpc_request);

    clientserver::Result idl_result;
    do {
        idl_result.m_result = 0;
        resetResult();
        mp_result_sub->wait_for_unread_message({10, 0});
        mp_result_sub->take_next_sample((void*)&m_rpc_response, &m_sampleInfo);
    } while (m_sampleInfo.instance_state != eprosima::fastdds::dds::ALIVE_INSTANCE_STATE ||
    m_rpc_response.header() != m_rpc_request.header());
    if (m_rpc_response.error_code() == soa_on_dds::SUCCESS)
    {
        if (!idl_result.DeserializeFromVector(m_rpc_response.response_payload())) {
            std::cout << "DeserializeFromVector failed" << std::endl;
            return soa_on_dds::DESERIALIZE_FAILED;
        }
        *result = idl_result.m_result;
    }
    return m_rpc_response.error_code();
}

void EprosimaClient::resetResult()
{
    m_rpc_response.header(soa_on_dds::RPC_Header{});
    m_rpc_response.response_payload().clear();
}

void EprosimaClient::OperationListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter*,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        mp_up->m_operationMatched++;
    }
    else if (info.current_count_change == -1)
    {
        mp_up->m_operationMatched--;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
    mp_up->isReady();
}

void EprosimaClient::ResultListener::on_subscription_matched(
        DataReader*,
        const SubscriptionMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        mp_up->m_resultMatched++;
    }
    else if (info.current_count_change == -1)
    {
        mp_up->m_resultMatched--;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
    mp_up->isReady();
}

void EprosimaClient::ResultListener::on_data_available(
        DataReader*)
{
}

bool EprosimaClient::isReady()
{
    if (m_operationMatched == 1 && m_resultMatched == 1)
    {
        m_isReady = true;
    }
    else
    {
        m_isReady = false;
    }
    return m_isReady;
}
