/**
 * @file RCObject.hpp
 * @brief 引用计数对象基类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-10-10
 *
 * @see More Effective C++, item 29
 */
#ifndef MINI_UTILS_RC_OBJECT_INC
#define MINI_UTILS_RC_OBJECT_INC

namespace mini_utils {

/**
 * @brief 引用计数对象基类, 管理引用计数
 */
class RCObject {
public:
    /**
     * @brief 默认构造
     */
    RCObject();

    /**
     * @brief 复制构造
     *
     * @param rhs 被复制对象
     */
    RCObject(const RCObject& rhs);

    /**
     * @brief 复制赋值
     *
     * @param rhs 被复制对象
     *
     * @return *this
     */
    RCObject& operator=(const RCObject& rhs);

    /**
     * @brief 析构函数, 子类需要覆盖
     */
    virtual ~RCObject() = 0;

    /**
     * @brief 增加引用计数
     */
    void addReference();

    /**
     * @brief 减少引用计数, 当引用计数减少到0时, delete this
     */
    void removeReference();

private:
    int refCount_;
};

}   // namespace mini_utils

#endif
