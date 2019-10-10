#ifndef MINI_UTILS_RC_PTR_INC
#define MINI_UTILS_RC_PTR_INC

namespace mini_utils {

template <class T>
class RCPtr {
public:
    RCPtr(T *realPtr = 0);
    RCPtr(const RCPtr &rhs);
    RCPtr& operator=(const RCPtr &rhs);
    ~RCPtr();

    T *operator ->() const;
    T &operator *() const;

private:
    T *pointee_; // dumb pointer this
                 // object is emulating
    void init(); // common initialization
};

template <class T>
RCPtr<T>::RCPtr(T *realPtr): pointee_(realPtr)
{
    init();
}

template <class T>
RCPtr<T>::RCPtr(const RCPtr &rhs): pointee_(rhs.pointee_)
{
    init();
}

template <class T>
void RCPtr<T>::init()
{
    if (pointee_ == 0) { // if the dumb pointer is
        return;          // null, so is the smart one
    }
    pointee_->addReference(); // note that there is now a
}                             // new reference to the value

template<class T>
RCPtr<T> &RCPtr<T>::operator =(const RCPtr &rhs)
{
    if (pointee_ != rhs.pointee_) { // skip assignments
        // where the value
        // doesn't change
        if (pointee_) {
            pointee_->removeReference(); // remove reference to
        } // current value
        pointee_ = rhs.pointee_; // point to new value
        init(); // if possible, share it
    } // else make own copy
    return *this;
}

template<class T>
RCPtr<T>::~RCPtr()
{
    if (pointee_) pointee_->removeReference();
}

template<class T>
T *RCPtr<T>::operator ->() const { return pointee_; }

template<class T>
T &RCPtr<T>::operator *() const { return *pointee_; }

}   // namespace mini_utils

#endif
