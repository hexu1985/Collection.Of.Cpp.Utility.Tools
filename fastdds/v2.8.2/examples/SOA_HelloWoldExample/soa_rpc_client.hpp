
namespace fastdds_soa {

class RpcClient {
public:
    RpcClient(const std::string& client_id, const std::string& service_name);

    void init();

    template <typename Argument, typename Result>
    int call(const std::string& method_name, const Argument& arg, Result& res);

    template <typename Argument, typename Result>
    void async_call(const std::string& method_name, const Argument& arg, 
            std::function<void(int, std::shared_ptr<Result>)> call_back);
};

}   // namespace fastdds_soa
