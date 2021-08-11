/**
 * @file RCPtr.hpp
 * @brief 侵入式引用计数指针, 类似于boost::intrusive_ptr
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-10-10
 *
 * @see More Effective C++, item 29
 */
#ifndef MINI_UTILS_RC_PTR_INC
#define MINI_UTILS_RC_PTR_INC

namespace mini_utils {

/**
 * @brief 侵入式引用计数指针
 *
 * @tparam T 被引用计数对象的类型, 需继承自RCObject或者和RCObject接口相同
 */
template <class T>
class RCPtr {
public:
    /**
     * @brief 构造函数
     *
     * @param realPtr 被引用计数对象的指针
     */
    RCPtr(T *realPtr = 0);

    /**
     * @brief 复制构造
     *
     * @param rhs 增加rhs的引用计数(如果rhs.pointee_不为空)
     */
    RCPtr(const RCPtr &rhs);

    /**
     * @brief 复制赋值
     *
     * @param rhs 首先减少*this的引用计数(如果pointee_不为空), 
     *            然后this->pointee_ = rhs.pointee_,
     *            然后增加rhs的引用计数(如果rhs不为空)
     *
     * @return *this
     */
    RCPtr& operator=(const RCPtr &rhs);

    /**
     * @brief 析构: 减少*this的引用计数
     */
    ~RCPtr();

    /**
     * @brief 重载->运算符, 获取引用计数对象的地址
     *
     * @return 引用计数对象的地址
     */
    T *operator ->() const;

    /**
     * @brief 重载*运算符, 获取引用计数对象的引用
     *
     * @return 引用计数对象的引用
     */
    T &operator *() const;

private:
    T *pointee_;
    void init();
};

template <class T>
void RCPtr<T>::init()
{
    if (pointee_ == 0) {
        return;
    }

    pointee_->addReference();
}

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

template<class T>
RCPtr<T> &RCPtr<T>::operator =(const RCPtr &rhs)
{
    if (pointee_ == rhs.pointee_) {
        return *this;
    }

    if (pointee_) {
        pointee_->removeReference();
    }

    pointee_ = rhs.pointee_;
    init();

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
