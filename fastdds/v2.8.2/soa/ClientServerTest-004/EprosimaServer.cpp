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
 * @file EprosimaServer.cpp
 *
 */

#include "EprosimaServer.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include "soa_on_dds_typesPubSubTypes.h"
#include "EprosimaParticipantManager.hpp"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;
using namespace clientserver;
using namespace std;

EprosimaServer::EprosimaServer()
    : mp_participant(nullptr)
    , m_n_served(0)
    , m_operationsListener(nullptr)
    , m_resultsListener(nullptr)
{
    m_operationsListener.mp_up = this;
    m_resultsListener.mp_up = this;

}

EprosimaServer::~EprosimaServer()
{
    mp_operation_sub.reset();
    mp_result_pub.reset();
    EprosimaParticipantManager::delete_participant(mp_participant);
}

void EprosimaServer::serve()
{
    cout << "Enter a number to stop the server: ";
    int aux;
    std::cin >> aux;
}

void EprosimaServer::serve(
        uint32_t samples)
{
    while (m_n_served < samples)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool EprosimaServer::init()
{
    if (!init_participant()) {
        return false;
    }

    if (!init_result_pub()) {
        return false;
    }

    if (!init_operation_sub()) {
        return false;
    }

    return true;
}

bool EprosimaServer::init_participant() {
    //CREATE THE PARTICIPANT
    DomainParticipantQos pqos;
    pqos.wire_protocol().builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol =
            eprosima::fastrtps::rtps::DiscoveryProtocol::SIMPLE;
    pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
    pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.name("server_RTPSParticipant");

    mp_participant = EprosimaParticipantManager::create_participant(0, pqos);

    if (mp_participant == nullptr)
    {
        return false;
    }

    return true;
}

bool EprosimaServer::init_operation_sub() {
    mp_operation_sub.reset(new EprosimaSubWrapper);
    EprosimaSubWrapper::Config config;
    config.participant = mp_participant;
    config.type_support.reset(new soa_on_dds::RPC_RequestPubSubType());
    config.topic_name = "soa.rpc.calculate.request";
    config.data_reader_listener = &this->m_operationsListener;

    DataReaderQos rqos;
    rqos.history().kind = KEEP_LAST_HISTORY_QOS;
    rqos.history().depth = 1000;
    rqos.resource_limits().max_samples = 1500;
    rqos.resource_limits().allocated_samples = 1000;
    config.data_reader_qos = rqos;

    if (!mp_operation_sub->init(config)) {
        return false;
    }

    return true;
}

bool EprosimaServer::init_result_pub() {
    mp_result_pub.reset(new EprosimaPubWrapper);
    EprosimaPubWrapper::Config config; 
    config.participant = mp_participant;
    config.type_support.reset(new soa_on_dds::RPC_ResponsePubSubType());
    config.topic_name = "soa.rpc.calculate.response";
    config.data_writer_listener = &this->m_resultsListener;

    DataWriterQos wqos;
    wqos.history().kind = KEEP_LAST_HISTORY_QOS;
    wqos.history().depth = 1000;
    wqos.resource_limits().max_samples = 1500;
    wqos.resource_limits().allocated_samples = 1000;
    wqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    config.data_writer_qos = wqos;

    if (!mp_result_pub->init(config)) {
        return false;
    }

    return true;
}

soa_on_dds::ErrorCode EprosimaServer::calculate(
        Operation::OPERATIONTYPE type,
        int32_t num1,
        int32_t num2,
        int32_t* result)
{
    switch (type)
    {
        case Operation::SUBTRACTION:
        {
            *result = num1 - num2;
            break;
        }
        case Operation::ADDITION:
        {
            *result = num1 + num2;
            break;
        }

        case Operation::MULTIPLICATION:
        {
            *result = num1 * num2;
            break;
        }
        case Operation::DIVISION:
        {
            if (num2 == 0)
            {
                return soa_on_dds::UPPER_LAYER_APPLICATION_ERROR;
            }
            *result = num1 / num2;
            break;
        }
    }
    return soa_on_dds::SUCCESS;
}

void EprosimaServer::OperationListener::on_data_available(
        DataReader* /*reader*/)
{
    //std::cout << "EprosimaServer::OperationListener::on_data_available" << std::endl;
    SampleInfo m_sampleInfo;
    mp_up->mp_operation_sub->take_next_sample((void*)&m_rpc_request, &m_sampleInfo);
    if (m_sampleInfo.valid_data)
    {
        ++mp_up->m_n_served;
        m_rpc_response.header(m_rpc_request.header());
        clientserver::Operation operation;
        soa_on_dds::ErrorCode ec = soa_on_dds::SUCCESS;
        int32_t result = 0;
        if (operation.DeserializeFromVector(m_rpc_request.request_payload())) {
            #if 0
            std::cout << "operation:  m_operationType=" << (uint32_t) operation.m_operationType 
                << ", m_num1=" << operation.m_num1
                << ", m_num2=" << operation.m_num2 << "\n";
            #endif
            ec = mp_up->calculate(
                operation.m_operationType,
                operation.m_num1,
                operation.m_num2,
                &result);
            //std::cout << "result: " << result << std::endl;
        } else {
            ec = soa_on_dds::SERVICE_DESERIALIZE_ERROR;
            std::cout << "operation.DeserializeFromVector failed!" << std::endl;
        }
        clientserver::Result idl_result;
        idl_result.m_result = result;
        std::vector<uint8_t> response_payload;
        if (idl_result.SerializeToVector(response_payload)) {
            m_rpc_response.response_payload(response_payload);
            //std::cout << "response_payload.size(): " << response_payload.size() << std::endl;
        } else {
            ec = soa_on_dds::SERVICE_SERIALIZE_ERROR;
            std::cout << "idl_result.SerializeToVector failed!" << std::endl;
        }
        m_rpc_response.error_code(ec);
        mp_up->mp_result_pub->write((void*)&m_rpc_response);
        //std::cout << "mp_up->mp_result_pub->write((void*)&m_rpc_response);" << std::endl;
    }
}
