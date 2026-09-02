#pragma once

#include "soa_on_dds_types.h"
#include "EprosimaPubWrapper.hpp"
#include "EprosimaSubWrapper.hpp"
#include "worker_thread.hpp"

#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include <unordered_map>
#include <string>
#include <future>
#include <atomic>
#include <memory>

namespace soa_on_dds {

class EprosimaRpcClient {
private:
    class IResponseProcessor {
    public:
        IResponseProcessor();
        virtual ~IResponseProcessor();
        
        virtual void process(std::shared_ptr<RPC_Response> rpc_response) = 0;
    };

    using IResponseProcessorPtr = std::shared_ptr<IResponseProcessor>;

    using RequestPtr = std::shared_ptr<soa_on_dds::RPC_Request>;
    using ResponsePtr = std::shared_ptr<soa_on_dds::RPC_Response>;
    using ResponsePromise = std::promise<ResponsePtr>;
    using ResponsePromisePtr = std::shared_ptr<ResponsePromise>;

    struct RequestInfo {
        RequestPtr request;
        ResponsePromisePtr response_promise;
        IResponseProcessorPtr response_processor;
        // call timestamps
    };
    
    using RequestInfoPtr = std::shared_ptr<RequestInfo>;

public:
    EprosimaRpcClient(const std::string& client_name, const std::string& service_name,
            eprosima::fastdds::dds::DomainParticipant* participant=nullptr);

    virtual ~EprosimaRpcClient();

    bool init();

    bool is_ready();

    template <typename Argument, typename Result>
    soa_on_dds::ErrorCode call(const std::string& method_name, const Argument& arg, Result& res) {
        return this->call_helper(method_name, arg, res, std::chrono::milliseconds::zero());
    }

    template <typename Argument, typename Result>
    soa_on_dds::ErrorCode call_timed(const std::string& method_name, const Argument& arg, Result& res,
            std::chrono::milliseconds timeout) {
        return this->call_helper(method_name, arg, res, timeout);
    }

    template <typename Argument, typename Result>
    void async_call(const std::string& method_name, const Argument& arg,
            std::function<void(soa_on_dds::ErrorCode, std::shared_ptr<Result>)> callback) {
        return this->async_call_helper(method_name, arg, callback, std::chrono::milliseconds::zero());
    }

    template <typename Argument, typename Result>
    void async_call(const std::string& method_name, const Argument& arg,
            std::function<void(soa_on_dds::ErrorCode, std::shared_ptr<Result>)> callback,
            std::chrono::milliseconds timeout) {
        return this->async_call_helper(method_name, arg, callback, timeout);
    }

private:
    template <typename Argument, typename Result>
    soa_on_dds::ErrorCode call_helper(const std::string& method_name, const Argument& arg, Result& res,
            std::chrono::milliseconds timeout) {
        if (!is_ready()) {
            return soa_on_dds::SERVICE_NOT_AVAILABLE;
        }

        std::vector<uint8_t> request_payload;
        if (!SerializeToVector(arg, request_payload)) {
            std::cout << "SerializeToVector failed" << std::endl;
            return soa_on_dds::CLIENT_SERIALIZE_ERROR;
        }

        ResponsePromisePtr response_promise = std::make_shared<ResponsePromise>();
        long request_id = send_request(method_name, request_payload, response_promise, nullptr);

        auto response_future = response_promise->get_future();
        auto rpc_response = response_future.get();
        assert(rpc_response != nullptr);

        if (rpc_response->error_code() != soa_on_dds::SUCCESS) {
            return rpc_response->error_code();
        }

        if (!DeserializeFromVector(res, rpc_response->response_payload())) {
            std::cout << "DeserializeFromVector failed" << std::endl;
            return soa_on_dds::CLIENT_DESERIALIZE_ERROR;
        }

        return soa_on_dds::SUCCESS;
    }

    template <typename Argument, typename Result>
    void async_call_helper(const std::string& method_name, const Argument& arg,
            std::function<void(soa_on_dds::ErrorCode, std::shared_ptr<Result>)> callback,
            std::chrono::milliseconds timeout) {
        if (!is_ready()) {
            callback(soa_on_dds::SERVICE_NOT_AVAILABLE, nullptr);
        }

        std::vector<uint8_t> request_payload;
        if (!SerializeToVector(arg, request_payload)) {
            callback(soa_on_dds::CLIENT_SERIALIZE_ERROR, nullptr);
        }

        IResponseProcessorPtr response_processor = std::make_shared<ResponseProcessor<Result>>(callback);
        send_request(method_name, request_payload, nullptr, response_processor);
    }

    EprosimaRpcClient(const EprosimaRpcClient&)=delete;
    const EprosimaRpcClient& operator =(const EprosimaRpcClient&)=delete;

    bool init_request_pub();
    bool init_response_sub();

    long send_request(const std::string& method_name, const std::vector<uint8_t>& request_payload,
        ResponsePromisePtr response_promise, IResponseProcessorPtr response_processor); 

    void on_data_available();

    void remove_pending_request(long request_id); 

    void do_send_request(RequestInfoPtr request_info);
    void do_recv_response();

    void do_remove_pending_request(long request_id);

    bool is_valid_response(ResponsePtr rpc_response);
    void dispatch_response(ResponsePtr rpc_response);

    class RequestPubListener : public eprosima::fastdds::dds::DataWriterListener {
    public:
        RequestPubListener(); 

        ~RequestPubListener() override;

        EprosimaRpcClient* m_up=nullptr;

        void on_publication_matched(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;
    }
    m_request_pub_listener;

    class ResponseSubListener : public eprosima::fastdds::dds::DataReaderListener {
    public:
        ResponseSubListener(); 

        ~ResponseSubListener() override;

        EprosimaRpcClient* m_up=nullptr;

        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override;

        void on_subscription_matched(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;
    }
    m_response_sub_listener;

    template <typename Result>
    class ResponseProcessor : public IResponseProcessor {
    public:
        ResponseProcessor(std::function<void(soa_on_dds::ErrorCode, std::shared_ptr<Result>)> callback): m_callback(callback) {};
        ~ResponseProcessor() override {}
        
        void process(std::shared_ptr<RPC_Response> rpc_response) override {
            if (rpc_response->error_code() != soa_on_dds::SUCCESS) {
                m_callback(rpc_response->error_code(), nullptr);
                return;
            }

            auto result = std::make_shared<Result>();
            if (!DeserializeFromVector(*result, rpc_response->response_payload())) {
                std::cout << "DeserializeFromVector failed" << std::endl;
                m_callback(soa_on_dds::CLIENT_DESERIALIZE_ERROR, nullptr);
                return;
            }

            m_callback(soa_on_dds::SUCCESS, result);
        }

        std::function<void(soa_on_dds::ErrorCode, std::shared_ptr<Result>)> m_callback;
    };

    std::atomic<bool> m_is_ready{false};

    std::atomic<int> m_request_pub_matched{0};

    std::atomic<int> m_response_sub_matched{0};

    std::string m_client_id;

    std::string m_service_name;

    long m_session_id{0};
    std::atomic<long> m_request_id_generator{0};

    eprosima::fastdds::dds::SampleInfo m_sample_info;
    ResponsePtr m_cached_response;
    std::unordered_map<long, RequestInfoPtr> m_pending_requests;        // key request_id

    RequestPtr make_rpc_request(const std::string& method_name, const std::vector<uint8_t>& request_payload);
    ResponsePtr make_rpc_response(RequestPtr rpc_request, soa_on_dds::ErrorCode error_code);
    RequestInfoPtr get_request_info(long request_id);

private:
    eprosima::fastdds::dds::DomainParticipant* m_participant=nullptr;
    std::unique_ptr<EprosimaPubWrapper> m_request_pub;
    std::unique_ptr<EprosimaSubWrapper> m_response_sub;
    worker_thread m_main_thread;
};

}   // namespace soa_on_dds
