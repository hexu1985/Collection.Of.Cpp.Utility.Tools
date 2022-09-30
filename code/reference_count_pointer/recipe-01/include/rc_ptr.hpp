/**
 * @file rc_ptr.hpp
 * @brief 侵入式引用计数指针, 类似于boost::intrusive_ptr
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-10-10
 *
 * @see More Effective C++, item 29
 */
#ifndef RC_PTR_INC
#define RC_PTR_INC

/**
 * @brief 侵入式引用计数指针, RC: reference count, Ptr: pointer
 *
 * @tparam T 被引用计数对象的类型, 需继承自RCObject或者和RCObject接口相同
 */
template <typename T>
class RCPtr {
public:
    /**
     * @brief 构造函数
     *
     * @param realPtr 被引用计数对象的指针
     */
    RCPtr(T* realPtr = 0);

    /**
     * @brief 复制构造
     *
     * @param rhs 增加rhs的引用计数(如果rhs.pointee不为空)
     */
    RCPtr(const RCPtr& rhs);

    /**
     * @brief 复制赋值
     *
     * @param rhs 首先减少*this的引用计数(如果pointee不为空), 
     *            然后this->pointee = rhs.pointee,
     *            然后增加rhs的引用计数(如果rhs不为空)
     *
     * @return *this
     */
    RCPtr& operator=(const RCPtr& rhs);

    /**
     * @brief 析构: 减少*this的引用计数
     */
    ~RCPtr();

    /**
     * @brief 重载->运算符, 获取引用计数对象的地址
     *
     * @return 引用计数对象的地址
     */
    T* operator->() const;

    /**
     * @brief 重载*运算符, 获取引用计数对象的引用
     *
     * @return 引用计数对象的引用
     */
    T& operator*() const;

private: 
    void addReference() const { if (pointee) pointee->addReference(); }
    void removeReference() const { if (pointee) pointee->removeReference(); }
    void init();

private:
    T* pointee = nullptr;
};

template <typename T>
void RCPtr<T>::init()
{
    addReference();
}

template <typename T>
RCPtr<T>::RCPtr(T* realPtr): pointee(realPtr)
{
    init();
}

template <typename T>
RCPtr<T>::RCPtr(const RCPtr& rhs): pointee(rhs.pointee)
{
    init();
}

template <typename T>
RCPtr<T>& RCPtr<T>::operator=(const RCPtr& rhs)
{
    rhs.addReference();
    this->removeReference();

    this->pointee = rhs.pointee;
    return *this;
}

template<typename T>
RCPtr<T>::~RCPtr()
{
    removeReference();
}

template<typename T>
T* RCPtr<T>::operator->() const { return pointee; }

template<typename T>
T& RCPtr<T>::operator*() const { return *pointee; }

#endif
