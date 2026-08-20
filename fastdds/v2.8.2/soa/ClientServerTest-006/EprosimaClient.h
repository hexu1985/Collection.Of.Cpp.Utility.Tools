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
 * @file EprosimaClient.h
 *
 */

#ifndef EPROSIMACLIENT_H_
#define EPROSIMACLIENT_H_

#include "soa_on_dds_types.h"
#include "ClientServerTypes.hpp"
#include "EprosimaRpcClient.hpp"

#include <memory>

class EprosimaClient
{
public:

    EprosimaClient();

    virtual ~EprosimaClient();

    bool init();

    soa_on_dds::ErrorCode calculate(
            clientserver::Operation::OPERATIONTYPE type,
            int32_t num1,
            int32_t num2,
            int32_t* result);

    bool isReady();

private:
    bool init_participant(); 
    bool init_rpc_client(); 

    eprosima::fastdds::dds::DomainParticipant* mp_participant;
    std::unique_ptr<soa_on_dds::EprosimaRpcClient> mp_rpc_client;
};

#endif /* EPROSIMACLIENT_H_ */
