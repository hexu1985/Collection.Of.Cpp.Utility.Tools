#pragma once

#include "soa_on_dds_types.h"
#include "EprosimaPubWrapper.hpp"
#include "EprosimaSubWrapper.hpp"
#include "worker_thread.hpp"
#include "thread_pool.hpp"

#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include <string>
#include <memory>
#include <cstdint>
#include <unordered_map>

namespace soa_on_dds {

class EprosimaRpcServer {
private:
    class IMethodHandler {
    public:
        IMethodHandler();
        virtual ~IMethodHandler();
        
        virtual soa_on_dds::ErrorCode process(const std::vector<uint8_t>& request_payload,
                    std::vector<uint8_t>& response_payload) = 0;
    };

    using IMethodHandlerPtr = std::shared_ptr<IMethodHandler>;

    using RequestPtr = std::shared_ptr<soa_on_dds::RPC_Request>;
    using ResponsePtr = std::shared_ptr<soa_on_dds::RPC_Response>;

public:
    EprosimaRpcServer(const std::string& service_name, size_t thread_pool_size, eprosima::fastdds::dds::DomainParticipant* participant=nullptr);

    virtual ~EprosimaRpcServer();

    bool start();

    void stop();

    template <typename Argument, typename Result>
    void register_method(const std::string& method_name,
            std::function<void(const Argument&, Result&)> process_function) {
        auto method_handler = std::make_shared<MethodHandler<Argument, Result>>(process_function);
        register_method_helper(method_name, method_handler);
    }

    void unregister_method(const std::string& method_name);

private:
    bool init_request_sub();
    bool init_response_pub();

    void register_method_helper(const std::string& method_name, IMethodHandlerPtr method_handler);
    void do_register_method(const std::string& method_name, IMethodHandlerPtr method_handler);

    void do_unregister_method(const std::string& method_name);

    void do_stop();

    void on_data_available();

    void do_recv_request();

    void do_send_response(ResponsePtr rpc_response);

    IMethodHandlerPtr get_method_handler(const std::string& method_name);

    ResponsePtr make_rpc_response(RequestPtr rpc_request, soa_on_dds::ErrorCode error_code);

    void process_request(IMethodHandlerPtr method_handler, RequestPtr rpc_request);

    class RequestSubListener : public eprosima::fastdds::dds::DataReaderListener {
    public:
        RequestSubListener();

        ~RequestSubListener() override;

        EprosimaRpcServer* m_up;

        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override;
    }
    m_request_sub_listener;

    class ResponsePubListener : public eprosima::fastdds::dds::DataWriterListener {
    public:
        ResponsePubListener();

        ~ResponsePubListener() override;

        EprosimaRpcServer* m_up;
    }
    m_response_pub_listener;

    template <typename Argument, typename Result>
    class MethodHandler: public IMethodHandler {
    public:
        MethodHandler(std::function<void(const Argument&, Result&)> process_function): m_process_function(process_function)  {}
        ~MethodHandler() override {}
        
        soa_on_dds::ErrorCode process(const std::vector<uint8_t>& request_payload,
                std::vector<uint8_t>& response_payload) {
            Argument arg;
            Result res;

            if (!DeserializeFromVector(arg, request_payload)) {
                std::cout << "DeserializeFromVector failed" << std::endl;
                return soa_on_dds::SERVICE_DESERIALIZE_ERROR;
            }

            try {
                m_process_function(arg, res);
            } catch (...) {
                return soa_on_dds::UPPER_LAYER_APPLICATION_ERROR;
            }

            if (!SerializeToVector(res, response_payload)) {
                std::cout << "SerializeToVector failed" << std::endl;
                return soa_on_dds::SERVICE_SERIALIZE_ERROR;
            }

            return soa_on_dds::SUCCESS;
        }

        std::function<void(const Argument&, Result&)> m_process_function;
    };

private:
    eprosima::fastdds::dds::DomainParticipant* m_participant=nullptr;
    std::unique_ptr<EprosimaSubWrapper> m_request_sub;
    std::unique_ptr<EprosimaPubWrapper> m_response_pub;
    std::string m_service_name;
    size_t m_thread_pool_size=0;
    worker_thread m_main_thread;
    thread_pool m_workers;
    std::unordered_map<std::string, IMethodHandlerPtr> m_method_handlers;
    eprosima::fastdds::dds::SampleInfo m_sample_info;
};

}   // namespace soa_on_dds
