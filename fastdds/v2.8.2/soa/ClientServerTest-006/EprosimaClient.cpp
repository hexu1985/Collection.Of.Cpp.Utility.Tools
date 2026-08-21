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

EprosimaClient::EprosimaClient()
    : mp_participant(nullptr)
{
}

EprosimaClient::~EprosimaClient()
{
    mp_rpc_client.reset();
    EprosimaParticipantManager::delete_participant(mp_participant);
}

bool EprosimaClient::init()
{
    if (!init_participant()) {
        return false;
    }

    if (!init_rpc_client()) {
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

bool EprosimaClient::init_rpc_client() {
    if (mp_participant == nullptr) {
        return false;
    }

    mp_rpc_client.reset(new soa_on_dds::EprosimaRpcClient("client", "compute", mp_participant));
    return mp_rpc_client->init();
}

soa_on_dds::ErrorCode EprosimaClient::calculate(
        OPERATIONTYPE type,
        int32_t num1,
        int32_t num2,
        int32_t* result) {
    if (mp_rpc_client == nullptr) {
        return soa_on_dds::CLIENT_NOT_INITIALIZE;
    }

    Operation operation;
    operation.m_operationType(type);
    operation.m_num1(num1);
    operation.m_num2(num2);

    Result idl_result;

    auto error_code = mp_rpc_client->call("operation", operation, idl_result);
    if (error_code != soa_on_dds::SUCCESS) {
        return error_code;
    }

    *result = idl_result.m_result();

    return soa_on_dds::SUCCESS;
}

bool EprosimaClient::isReady() {
    if (mp_rpc_client == nullptr) {
        return false;
    }

    return mp_rpc_client->is_ready();
}
