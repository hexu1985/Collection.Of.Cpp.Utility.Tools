
namespace fdds {

class DDSPublisherBase {
public:

protected:
    std::shared_ptr<DDSPublisherImpl> pub_impl_;
};

}   // namespace fdds
