#ifndef MINI_UTILS_RC_OBJECT_INC
#define MINI_UTILS_RC_OBJECT_INC

namespace mini_utils {

class RCObject {
public:
    RCObject();
    RCObject(const RCObject& rhs);
    RCObject& operator=(const RCObject& rhs);
    virtual ~RCObject() = 0;

    void addReference();
    void removeReference();

private:
    int refCount_;
};

}   // namespace mini_utils

#endif
