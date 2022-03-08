/**
 * @file singleton.hpp
 * @brief 一个单件类模板
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-03-02
 */
#ifndef SINGLETON_INC
#define SINGLETON_INC

#include <mutex>

/**
 * @brief 单件类模板
 *
 * @tparam T 实际单件类型
 */
template <typename T>
class Singleton {
public:
    /**
     * @brief 获取单件实例
     *
     * @return 实例引用
     */
    static T &getInstance()
    {
        static T instance;
        return instance;
    }

protected:
    Singleton() = default;

private:
    Singleton(const Singleton &) = delete;
    Singleton &operator =(const Singleton &) = delete;
};

#endif
