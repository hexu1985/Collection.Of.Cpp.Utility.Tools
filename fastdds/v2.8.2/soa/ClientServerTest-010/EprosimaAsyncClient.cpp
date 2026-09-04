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
 * @file EprosimaAsyncClient.cpp
 *
 */

#include "EprosimaAsyncClient.h"

#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <fastdds/rtps/common/Guid.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;
using namespace clientserver;

EprosimaAsyncClient::EprosimaAsyncClient()
{
}

EprosimaAsyncClient::~EprosimaAsyncClient()
{
    mp_rpc_client.reset();
}

bool EprosimaAsyncClient::init()
{
    //std::cout << "EprosimaAsyncClient::init_rpc_client" << std::endl;
    mp_rpc_client = soa_on_dds::EprosimaRpcClient::create_rpc_client("client", "compute.service");
    return mp_rpc_client->init();
}

void EprosimaAsyncClient::calculate(
        OPERATIONTYPE type,
        int32_t num1,
        int32_t num2,
        std::function<void(int)> result_callback) {
    if (mp_rpc_client == nullptr) {
        std::cout << "mp_rpc_client is nullptr" << std::endl;
        return; 
    }

    Operation operation;
    operation.m_operationType(type);
    operation.m_num1(num1);
    operation.m_num2(num2);

    mp_rpc_client->async_call<Operation, Result>("operation", operation, 
            [this, result_callback](soa_on_dds::ErrorCode error, std::shared_ptr<Result> result){
                if (error != soa_on_dds::SUCCESS) {
                    std::cout << "error: " << static_cast<uint32_t>(error) << std::endl;
                    return;
                }
                result_callback(result->m_result());
            });
}

bool EprosimaAsyncClient::isReady() {
    if (mp_rpc_client == nullptr) {
        return false;
    }

    return mp_rpc_client->is_ready();
}

void EprosimaAsyncClient::wait()
{
    std::cout << "Enter a number to stop the server: ";
    int aux;
    std::cin >> aux;
}
