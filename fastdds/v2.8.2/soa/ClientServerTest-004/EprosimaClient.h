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
#include "EprosimaPubWrapper.hpp"
#include "EprosimaSubWrapper.hpp"

#include <memory>

class EprosimaClient
{
public:

    EprosimaClient();

    virtual ~EprosimaClient();

    eprosima::fastdds::dds::DomainParticipant* mp_participant;

    std::unique_ptr<EprosimaPubWrapper> mp_operation_pub;
    std::unique_ptr<EprosimaSubWrapper> mp_result_sub;

    bool init();

    soa_on_dds::ErrorCode calculate(
            clientserver::Operation::OPERATIONTYPE type,
            int32_t num1,
            int32_t num2,
            int32_t* result);

    bool isReady();

private:
    bool init_participant(); 
    bool init_operation_pub();
    bool init_result_sub();

    soa_on_dds::RPC_Request m_rpc_request;
    soa_on_dds::RPC_Response m_rpc_response;

    void resetResult();

    class OperationListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:

        OperationListener(
                EprosimaClient* up)
            : mp_up(up)
        {
        }

        ~OperationListener() override
        {
        }

        EprosimaClient* mp_up;

        void on_publication_matched(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;


    }
    m_operationsListener;

    class ResultListener : public eprosima::fastdds::dds::DataReaderListener
    {
    public:

        ResultListener(
                EprosimaClient* up)
            : mp_up(up)
        {
        }

        ~ResultListener() override
        {
        }

        EprosimaClient* mp_up;

        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override;

        void on_subscription_matched(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

    }
    m_resultsListener;

    bool m_isReady;

    int m_operationMatched;

    int m_resultMatched;

};

#endif /* EPROSIMACLIENT_H_ */
