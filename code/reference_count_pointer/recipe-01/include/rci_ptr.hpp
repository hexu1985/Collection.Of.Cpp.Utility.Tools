/**
 * @file rc_ptr.hpp
 * @brief 引用计数指针, 类似于std::shared_ptr
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-10-10
 *
 * @see 提高C++性能的编程技术, 第12章
 */
#ifndef RCI_PTR_INC
#define RCI_PTR_INC

#include "rc_object.hpp"

/**
 * @brief 引用计数指针, RC: reference count, I: indirect, Ptr: pointer
 *
 * @tparam T 被引用计数对象的类型
 */
template <class T>
class RCIPtr {
public:
    /**
     * @brief 构造函数
     *
     * @param realPtr 被引用计数对象的指针
     */
    RCIPtr(T* realPtr = 0);

    /**
     * @brief 复制构造
     *
     * @param rhs 增加rhs的引用计数(如果rhs.counter不为空)
     */
    RCIPtr(const RCIPtr& rhs);

    /**
     * @brief 复制赋值
     *
     * @param rhs 首先减少*this的引用计数(如果counter不为空), 
     *            然后this->counter = rhs.counter,
     *            然后增加rhs的引用计数(如果rhs不为空)
     *
     * @return *this
     */
    RCIPtr& operator=(const RCIPtr& rhs);

    /**
     * @brief 析构: 减少*this的引用计数
     */
    ~RCIPtr();

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
    void addReference() const { if (counter) counter->addReference(); }
    void removeReference() const { if (counter) counter->removeReference(); }
    void init();

private:
    struct CountHolder: public RCObject {
        CountHolder(T* realPtr): pointee(realPtr) {}
        ~CountHolder() { delete pointee; }
        T* pointee;
    };

    CountHolder* counter=nullptr;
};

template <class T>
void RCIPtr<T>::init()
{
    if (counter == 0) { 
        return;
    }
    counter->addReference();
}

template <class T>
RCIPtr<T>::RCIPtr(T* realPtr): counter(new CountHolder(realPtr))
{
    init();
}

template <class T>
RCIPtr<T>::RCIPtr(const RCIPtr& rhs): counter(rhs.counter)
{
    init();
}

template<class T>
RCIPtr<T>& RCIPtr<T>::operator=(const RCIPtr& rhs)
{
    rhs.addReference();
    this->removeReference();

    this->counter = rhs.counter;
    return *this;
}

template<class T>
RCIPtr<T>::~RCIPtr()
{
    removeReference();
}

template<class T>
T* RCIPtr<T>::operator->() const { return counter->pointee; }

template<class T>
T& RCIPtr<T>::operator*() const { return *(counter->pointee); }

#endif
