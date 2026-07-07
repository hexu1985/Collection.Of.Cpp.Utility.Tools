
namespace fastdds_soa {

class RpcService {
public:
    RpcClient(const std::string& service_name);

    template <typename Argument, typename Result>
    void register_method(const std::string& method_name, 
            std::function<void (const Argument&, Result& )> process_function);

    void unregister_method(const std::string& method_name);

    void start();
    void stop();
};

}   // namespace fastdds_soa

