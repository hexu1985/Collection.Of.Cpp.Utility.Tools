/**
 * @file Singleton.hpp
 * @brief 一个单件类模板
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2020-03-02
 */
#ifndef MINI_UTILS_SINGLETON_INC
#define MINI_UTILS_SINGLETON_INC

#include <mutex>

namespace mini_utils {

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

private:
    Singleton(const Singleton &) = delete;
    Singleton &operator =(const Singleton &) = delete;
};

}   // namespace mini_utils

#endif
