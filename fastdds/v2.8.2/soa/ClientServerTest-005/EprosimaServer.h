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
 * @file EprosimaServer.h
 *
 */

#ifndef EPROSIMASERVER_H_
#define EPROSIMASERVER_H_

#include "soa_on_dds_types.h"
#include "ClientServerTypes.hpp"
#include "EprosimaRpcServer.hpp"

#include <memory>

class EprosimaServer
{
public:

    EprosimaServer();

    virtual ~EprosimaServer();

    bool init();

    //Serve indefinitely.
    void serve();

    //Serve for samples operations.
    void serve(
            uint32_t samples);

    void operation_handle(
            const clientserver::Operation &operation, 
            clientserver::Result& result);

private:
    bool init_participant(); 
    bool init_rpc_server(); 

    eprosima::fastdds::dds::DomainParticipant* mp_participant;

    std::unique_ptr<soa_on_dds::EprosimaRpcServer> mp_rpc_server;

    soa_on_dds::ErrorCode calculate(
            clientserver::Operation::OPERATIONTYPE type,
            int32_t num1,
            int32_t num2,
            int32_t* result);

public:

    uint32_t m_n_served;
};

#endif /* EPROSIMASERVER_H_ */
