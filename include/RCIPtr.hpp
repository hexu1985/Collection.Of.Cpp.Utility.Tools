#ifndef MINI_UTILS_RCI_PTR_INC
#define MINI_UTILS_RCI_PTR_INC

#include "RCObject.hpp"

namespace mini_utils {

template <class T>
class RCIPtr {
public:
    RCIPtr(T *realPtr = 0);
    RCIPtr(const RCIPtr &rhs);
    RCIPtr& operator=(const RCIPtr &rhs);
    ~RCIPtr();

    T *operator ->() const;
    T &operator *() const;

private:
    struct CountHolder: public RCObject {
        ~CountHolder() { delete pointee; }
        T *pointee; // dumb pointer this
                    // object is emulating
    };

    CountHolder *counter_;
    void init(); // common initialization
};

template <class T>
RCIPtr<T>::RCIPtr(T *realPtr): counter_(new CountHolder)
{
    counter_->pointee = realPtr;
    init();
}

template <class T>
RCIPtr<T>::RCIPtr(const RCIPtr &rhs): counter_(rhs.counter_)
{
    init();
}

template <class T>
void RCIPtr<T>::init()
{
    if (counter_ == 0) { 
        return;
    }
    counter_->addReference(); // note that there is now a
}                             // new reference to the value

template<class T>
RCIPtr<T> &RCIPtr<T>::operator =(const RCIPtr &rhs)
{
    if (counter_ != rhs.counter_) { // skip assignments
        // where the value
        // doesn't change
        if (counter_) {
            counter_->removeReference(); // remove reference to
        } // current value
        counter_ = rhs.counter_; // point to new value
        init(); // if possible, share it
    } // else make own copy
    return *this;
}

template<class T>
RCIPtr<T>::~RCIPtr()
{
    if (counter_) counter_->removeReference();
}

template<class T>
T *RCIPtr<T>::operator ->() const { return counter_->pointee; }

template<class T>
T &RCIPtr<T>::operator *() const { return *(counter_->pointee); }

}   // namespace mini_utils

#endif
