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

#include "ClientServerTypes.h"
#include "EprosimaPubWrapper.hpp"
#include "EprosimaSubWrapper.hpp"

#include <memory>

class EprosimaServer
{
    friend class OperationListener;
    friend class ResultListener;

public:

    EprosimaServer();

    virtual ~EprosimaServer();

    bool init();

    //Serve indefinitely.
    void serve();

    //Serve for samples operations.
    void serve(
            uint32_t samples);

private:
    bool init_operation_sub(); 
    bool init_result_pub(); 

    eprosima::fastdds::dds::DomainParticipant* mp_participant;

    std::unique_ptr<EprosimaSubWrapper> mp_operation_sub;
    std::unique_ptr<EprosimaPubWrapper> mp_result_pub;

    clientserver::RESULTTYPE calculate(
            clientserver::OPERATIONTYPE type,
            int32_t num1,
            int32_t num2,
            int32_t* result);

public:

    uint32_t m_n_served;

    class OperationListener : public eprosima::fastdds::dds::DataReaderListener
    {
    public:

        OperationListener(
                EprosimaServer* up)
            : mp_up(up)
        {
        }

        ~OperationListener() override
        {
        }

        EprosimaServer* mp_up;

        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override;

        clientserver::Operation m_operation;

        clientserver::Result m_result;

    }
    m_operationsListener;

    class ResultListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:

        ResultListener(
                EprosimaServer* up)
            : mp_up(up)
        {
        }

        ~ResultListener() override
        {
        }

        EprosimaServer* mp_up;
    }
    m_resultsListener;
};

#endif /* EPROSIMASERVER_H_ */
