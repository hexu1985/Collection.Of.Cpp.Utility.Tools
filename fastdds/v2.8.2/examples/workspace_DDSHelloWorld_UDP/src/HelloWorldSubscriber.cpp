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
#include "CustomDomainParticipantListener.hpp"

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
#include <fastdds/dds/log/FileConsumer.hpp>

#include "cxxopts.hpp"
#include "worker_thread.hpp"

using eprosima::fastrtps::Duration_t;
using namespace eprosima::fastdds::dds;

class HelloWorldSubscriber
{
private:

    DomainParticipant* participant_;

    Subscriber* subscriber_;

    Topic* topic_;

    DataReader* reader_;

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
            if (sleep_ != 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleep_));
            }

            if (skip_read_) {
                std::cout << "skip read sample" << std::endl;
                return;
            }

            if (async_) {
                worker_.submit([=]() { 
                        std::cout << "async read sample" << std::endl;
                        read_sample(reader); });
            } else{
                std::cout << "A data sample was available" << std::endl;
                read_sample(reader);
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

        void read_sample(DataReader* reader)
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
            if (sleep_after_read_ != 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleep_after_read_));
            }
        }

        HelloWorld hello_;

        std::atomic_uint samples_;

        uint32_t sleep_=0;    // milliseconds

        bool skip_read_=false;

        bool async_=false;

        uint32_t sleep_after_read_=0;    // milliseconds

        worker_thread worker_;
    } listener_;

    CustomDomainParticipantListener participant_listener_;

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

        // 参与者活跃租期
        if (options_["lease_duration"].as<int>() > 0) {
            participantQos.wire_protocol().builtin.discovery_config.leaseDuration = 
                Duration_t(options_["lease_duration"].as<int>(), 0);
        }
        std::cout << "builtin.discovery_config.leaseDuration: " 
                  << participantQos.wire_protocol().builtin.discovery_config.leaseDuration
                  << std::endl;

        // 参与者存活公告周期，应小于活跃租期
        if (options_["announcement_period"].as<int>() > 0) {
            participantQos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = 
                Duration_t(options_["announcement_period"].as<int>(), 0);
        }
        std::cout << "builtin.discovery_config.leaseDuration_announcementperiod: " 
                  << participantQos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod
                  << std::endl;

        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos, &participant_listener_, StatusMask::none());

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

        DataReaderQos readerQos;
        readerQos.history().kind = KEEP_LAST_HISTORY_QOS;
        readerQos.history().depth = options_["history"].as<int>();  // 只保留最后n条消息
        std::cout << "readerQos.history().depth: " << readerQos.history().depth << std::endl;

        // 设置可靠性为可靠的
        readerQos.reliability().kind = RELIABLE_RELIABILITY_QOS;

        // Create the DataReader
        listener_.sleep_ = options_["sleep"].as<uint32_t>();
        listener_.skip_read_ = options_["skip_read"].as<bool>();
        listener_.async_ = options_["async"].as<bool>();
        listener_.sleep_after_read_ = options_["sleep_after_read"].as<uint32_t>();
        std::cout << "sleep: " << listener_.sleep_ << std::endl;
        std::cout << "skip_read: " << listener_.skip_read_ << std::endl;
        std::cout << "async: " << listener_.async_ << std::endl;
        std::cout << "sleep_after_read: " << listener_.sleep_after_read_ << std::endl;

        reader_ = subscriber_->create_datareader(topic_, readerQos, &listener_);

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

void init_log(bool verbose, const std::string& log_file) {
    // log info
    if (verbose)
        Log::SetVerbosity(Log::Kind::Info);
    else
        Log::SetVerbosity(Log::Kind::Warning);
    //Log::SetCategoryFilter(std::regex("(RTPS_|SECURITY_)"));  // 可选: 设置类别过滤器

    // 启用文件名和行号显示
    Log::ReportFilenames(true);  // 显示文件名
    Log::ReportFunctions(true);  // 显示函数名（可选）

    if (!log_file.empty()) {
        std::unique_ptr<FileConsumer> file_consumer{new FileConsumer{log_file}};
        Log::RegisterConsumer(std::move(file_consumer));
    }
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
        ("history", "Depth of history", cxxopts::value<int>()->default_value("5"))
        ("sleep", "sleep milliseconds on data available", cxxopts::value<uint32_t>()->default_value("0"))
        ("skip_read", "skip reading on data available", cxxopts::value<bool>()->default_value("false"))
        ("async", "read data asynchronous", cxxopts::value<bool>()->default_value("false"))
        ("sleep_after_read", "sleep milliseconds after read sample", cxxopts::value<uint32_t>()->default_value("0"))
        ("log_file", "the path of log file, default empty(no log file to write)", cxxopts::value<std::string>()->default_value(""))
        ("lease_duration", "discovery_config.leaseDuration, unit second", cxxopts::value<int>()->default_value("-1"))
        ("announcement_period", "discovery_config.leaseDuration_announcementperiod, unit second", cxxopts::value<int>()->default_value("-1"))
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
        std::string log_file = result["log_file"].as<std::string>();

        // 使用参数...
        init_log(verbose, log_file);
        
        std::cout << "Starting subscriber." << std::endl;

        logWarning(main, "Starting subscriber.");

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
