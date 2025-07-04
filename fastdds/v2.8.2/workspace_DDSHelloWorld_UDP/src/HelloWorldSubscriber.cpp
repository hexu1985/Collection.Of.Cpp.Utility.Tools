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
 * @file HelloWorldSubscriber.cpp
 *
 */

#include "HelloWorldPubSubTypes.h"

#include <chrono>
#include <thread>
#include <memory>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>

#include <fastdds/dds/log/Log.hpp>

#include "cxxopts.hpp"

using namespace eprosima::fastdds::dds;

class HelloWorldSubscriber
{
private:

    DomainParticipant* participant_;

    Subscriber* subscriber_;

    DataReader* reader_;

    Topic* topic_;

    TypeSupport type_;

    const cxxopts::ParseResult& options_;

    class SubListener : public DataReaderListener
    {
    public:

        SubListener()
            : samples_(0)
        {
        }

        ~SubListener() override
        {
        }

        void on_subscription_matched(
                DataReader*,
                const SubscriptionMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                std::cout << "Subscriber matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                std::cout << "Subscriber unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                          << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
            }
        }

        void on_data_available(
                DataReader* reader) override
        {
            SampleInfo info;
            if (reader->take_next_sample(&hello_, &info) == eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK)
            {
                if (info.valid_data)
                {
                    samples_++;
                    std::cout << "Message: " << hello_.message() << " with index: " << hello_.index()
                              << " RECEIVED." << std::endl;
                }
            }
        }

        void on_requested_deadline_missed(
                DataReader* reader,
                const eprosima::fastrtps::RequestedDeadlineMissedStatus& info) override
        {
            (void)reader, (void)info;
            std::cout << "Some data was not received on time" << std::endl;
        }

        void on_liveliness_changed(
                DataReader* reader,
                const eprosima::fastrtps::LivelinessChangedStatus& info) override
        {
            (void)reader;
            if (info.alive_count_change == 1)
            {
                std::cout << "A matched DataWriter has become active" << std::endl;
            }
            else if (info.not_alive_count_change == 1)
            {
                std::cout << "A matched DataWriter has become inactive" << std::endl;
            }
        }

        void on_sample_rejected(
                DataReader* reader,
                const eprosima::fastrtps::SampleRejectedStatus& info) override
        {
            (void)reader, (void)info;
            std::cout << "A received data sample was rejected" << std::endl;
        }

        void on_requested_incompatible_qos(
                DataReader* /*reader*/,
                const RequestedIncompatibleQosStatus& info) override
        {
            std::cout << "Found a remote Topic with incompatible QoS (QoS ID: " << info.last_policy_id <<
                ")" << std::endl;
        }

        void on_sample_lost(
                DataReader* reader,
                const SampleLostStatus& info) override
        {
            (void)reader, (void)info;
            std::cout << "A data sample was lost and will not be received" << std::endl;
        }

        HelloWorld hello_;

        std::atomic_int samples_;

    }
    listener_;

public:

    HelloWorldSubscriber(const cxxopts::ParseResult& options)
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new HelloWorldPubSubType())
        , options_(options)
    {
    }

    virtual ~HelloWorldSubscriber()
    {
        if (reader_ != nullptr)
        {
            subscriber_->delete_datareader(reader_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        if (subscriber_ != nullptr)
        {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Initialize the subscriber
    bool init()
    {
        DomainParticipantQos participantQos;
        participantQos.name("Participant_subscriber");

        if (options_["udp_only"].as<bool>()) {
            std::cout << "only use udp transport" << std::endl;
            participantQos.transport().use_builtin_transports = false;  // 禁用所有内置传输

            // 只启用 UDPv4 传输
            auto udp_transport = std::make_shared<eprosima::fastdds::rtps::UDPv4TransportDescriptor>();
            participantQos.transport().user_transports.push_back(udp_transport);
        }

        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (participant_ == nullptr)
        {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("HelloWorldTopic", "HelloWorld", TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr)
        {
            return false;
        }

        // Create the Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (subscriber_ == nullptr)
        {
            return false;
        }

        // Create the DataReader
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

        if (reader_ == nullptr)
        {
            return false;
        }

        return true;
    }

    //!Run the Subscriber
    void run(
            uint32_t samples)
    {
        while (listener_.samples_ < samples)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

};

void init_log(bool verbose) {
    // log info
    if (verbose)
        Log::SetVerbosity(Log::Kind::Info);
    else
        Log::SetVerbosity(Log::Kind::Warning);
    //Log::SetCategoryFilter(std::regex("(RTPS_|SECURITY_)"));  // 可选: 设置类别过滤器

    // 启用文件名和行号显示
    Log::ReportFilenames(true);  // 显示文件名
    Log::ReportFunctions(true);  // 显示函数名（可选）
}

int main(
        int argc,
        char** argv)
{
    cxxopts::Options options("HelloWorldSubscriber", "A brief description of HelloWorldSubscriber");
    // 添加选项
    options.add_options()
        ("h,help", "Print help")
        ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
        ("udp_only", "only use udp transport", cxxopts::value<bool>()->default_value("false"))
        ("n,number", "Number of iterations", cxxopts::value<int>()->default_value("10"))
        ;

    try {
        // 解析命令行参数
        cxxopts::ParseResult result = options.parse(argc, argv);

        // 处理帮助选项
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        // 获取参数值
        bool verbose = result["verbose"].as<bool>();
        int samples = result["number"].as<int>();

        // 使用参数...
        init_log(verbose);
        
        std::cout << "Starting subscriber." << std::endl;

        std::unique_ptr<HelloWorldSubscriber> mysub{new HelloWorldSubscriber(result)};
        if (mysub->init())
        {
            mysub->run(samples);
        }

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
