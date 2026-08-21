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

#include <stdexcept>
#include <functional>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include "soa_on_dds_typesPubSubTypes.h"
#include "EprosimaParticipantManager.hpp"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;
using namespace clientserver;
using namespace std;
using namespace std::placeholders;

EprosimaServer::EprosimaServer()
    : mp_participant(nullptr)
    , m_n_served(0)
{
}

EprosimaServer::~EprosimaServer()
{
    mp_rpc_server.reset();
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

    if (!init_rpc_server()) {
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

bool EprosimaServer::init_rpc_server() {
    if (mp_participant == nullptr) {
        return false;
    }

    mp_rpc_server.reset(new soa_on_dds::EprosimaRpcServer("compute", 3, mp_participant));
    mp_rpc_server->register_method<Operation, Result>("operation", std::bind(&EprosimaServer::operation_handle, this, _1, _2));
    return mp_rpc_server->start();
}

void EprosimaServer::operation_handle(
        const Operation &operation, 
        Result& result) 
{
    int32_t m_result = 0;
    if (calculate(operation.m_operationType(), operation.m_num1(), operation.m_num2(), &m_result) != soa_on_dds::SUCCESS) {
        throw std::runtime_error("calculate error");
    }
    result.m_result(m_result);
}

soa_on_dds::ErrorCode EprosimaServer::calculate(
        OPERATIONTYPE type,
        int32_t num1,
        int32_t num2,
        int32_t* result)
{
    std::cout << "EprosimaServer::calculate" << std::endl;
    switch (type)
    {
        case SUBTRACTION:
        {
            *result = num1 - num2;
            break;
        }
        case ADDITION:
        {
            *result = num1 + num2;
            break;
        }

        case MULTIPLICATION:
        {
            *result = num1 * num2;
            break;
        }
        case DIVISION:
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

