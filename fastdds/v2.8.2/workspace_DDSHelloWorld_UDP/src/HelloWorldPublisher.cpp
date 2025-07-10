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
 * @file HelloWorldPublisher.cpp
 *
 */

#include "HelloWorldPubSubTypes.h"

#include <chrono>
#include <thread>
#include <memory>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>

#include <fastdds/dds/log/Log.hpp>
#include <fastdds/dds/log/FileConsumer.hpp>

#include "cxxopts.hpp"

using namespace eprosima::fastdds::dds;

class HelloWorldPublisher
{
private:
    HelloWorld hello_;

    DomainParticipant* participant_;

    Publisher* publisher_;

    Topic* topic_;

    DataWriter* writer_;

    TypeSupport type_;

    const cxxopts::ParseResult& options_;

    class PubListener : public DataWriterListener
    {
    public:

        PubListener()
            : matched_(0)
        {
        }

        ~PubListener() override
        {
        }

        void on_publication_matched(
                DataWriter*,
                const PublicationMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                matched_ = info.current_count;
                std::cout << "Publisher matched. matched_: " << matched_ << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                matched_ = info.current_count;
                std::cout << "Publisher unmatched. matched_: " << matched_ << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                        << " is not a valid value for PublicationMatchedStatus current count change." << std::endl;
            }
        }

        void on_offered_deadline_missed(
                DataWriter* writer,
                const OfferedDeadlineMissedStatus& status) override
        {
            (void)writer, (void)status;
            std::cout << "Some data could not be delivered on time" << std::endl;
        }

        void on_offered_incompatible_qos(
                DataWriter* /*writer*/,
                const OfferedIncompatibleQosStatus& status) override
        {
            std::cout << "Found a remote Topic with incompatible QoS (QoS ID: " << status.last_policy_id <<
                ")" << std::endl;
        }

        void on_liveliness_lost(
                DataWriter* writer,
                const LivelinessLostStatus& status) override
        {
            (void)writer, (void)status;
            std::cout << "Liveliness lost. Matched Subscribers will consider us offline" << std::endl;
        }

        std::atomic_int matched_;

    } listener_;

public:

    
    HelloWorldPublisher(const cxxopts::ParseResult& options)
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new HelloWorldPubSubType())
        , options_(options)
    {
    }

    virtual ~HelloWorldPublisher()
    {
        if (writer_ != nullptr)
        {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_ != nullptr)
        {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Initialize the publisher
    bool init()
    {
        hello_.index(0);
        hello_.message("HelloWorld");

        DomainParticipantQos participantQos;
        participantQos.name("Participant_publisher");

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

        // Create the publications Topic
        topic_ = participant_->create_topic("HelloWorldTopic", "HelloWorld", TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr)
        {
            return false;
        }

        // Create the Publisher
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

        if (publisher_ == nullptr)
        {
            return false;
        }

        DataWriterQos writerQos = DATAWRITER_QOS_DEFAULT;
        writerQos.history().kind = KEEP_LAST_HISTORY_QOS;
        writerQos.history().depth = options_["history"].as<int>();  // 只保留最后n条消息
        std::cout << "writerQos.history().depth: " << writerQos.history().depth << std::endl;

        // 设置可靠性为可靠的
        writerQos.reliability().kind = RELIABLE_RELIABILITY_QOS;

        // Create the DataWriter
        writer_ = publisher_->create_datawriter(topic_, writerQos, &listener_);

        if (writer_ == nullptr)
        {
            return false;
        }
        return true;
    }

    //!Send a publication
    bool publish()
    {
        if (listener_.matched_ > 0)
        {
            hello_.index(hello_.index() + 1);
            hello_.message("HelloWorld:" + std::to_string(hello_.index()));
            if (writer_->write(&hello_)) {
                return true;
            } else {
                std::cout << "Message: " << hello_.message() << " with index: " << hello_.index()
                            << " SENT failed!" << std::endl;
            }
        }
        return false;
    }

    //!Run the Publisher
    void run(
            uint32_t samples,
            uint32_t sleep)
    {
        uint32_t samples_sent = 0;
        while (samples_sent < samples)
        {
            if (publish())
            {
                samples_sent++;
                std::cout << "Message: " << hello_.message() << " with index: " << hello_.index()
                            << " SENT success!" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
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
    cxxopts::Options options("HelloWorldPublisher", "A brief description of HelloWorldPublisher");
    // 添加选项
    options.add_options()
        ("h,help", "Print help")
        ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
        ("udp_only", "only use udp transport", cxxopts::value<bool>()->default_value("false"))
        ("n,number", "Number of iterations", cxxopts::value<int>()->default_value("10"))
        ("history", "Depth of history", cxxopts::value<int>()->default_value("5"))
        ("sleep", "sleep milliseconds between writing", cxxopts::value<uint32_t>()->default_value("1000"))
        ("log_file", "the path of log file, default empty(no log file to write)", cxxopts::value<std::string>()->default_value(""))
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
        uint32_t sleep = result["sleep"].as<uint32_t>();
        std::string log_file = result["log_file"].as<std::string>();

        // 使用参数...
        init_log(verbose, log_file);
        
        std::cout << "Starting publisher." << std::endl;

        std::unique_ptr<HelloWorldPublisher> mypub{new HelloWorldPublisher(result)};
        if(mypub->init())
        {
            mypub->run(samples, sleep);
        }

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
