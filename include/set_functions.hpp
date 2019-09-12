/**
 * @file set_functions.hpp
 * @brief 集合相关的函数
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-09-12
 */
#ifndef MINI_UTILS_SET_FUNCTIONS_INC
#define MINI_UTILS_SET_FUNCTIONS_INC

#include <vector>
#include <algorithm>

namespace mini_utils {

/**
 * @brief 计算集合交集
 *
 * @tparam T 集合元素类型
 * @param v1 集合1
 * @param v2 集合2
 *
 * @return 集合1和集合2的交集
 */
template <typename T>
std::vector<T> set_intersection(const std::vector<T> &v1, const std::vector<T> &v2)
{
    int n1 = v1.size();
    int n2 = v2.size();
    int i1 = 0;
    int i2 = 0;

    std::vector<T> res;
    res.reserve(std::min(n1, n2));
    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            i1++;
        } else if (v2[i2] < v1[i1]) {
            i2++;
        } else {
            res.push_back(v1[i1]);
            i1++;
            i2++;
        }
    }

    return res;
}

/**
 * @brief 计算集合并集
 *
 * @tparam T 集合元素类型
 * @param v1 集合1
 * @param v2 集合2
 *
 * @return 集合1和集合2的并集
 */
template <typename T>
std::vector<T> set_union(const std::vector<T> &v1, const std::vector<T> &v2)
{
    int n1 = v1.size();
    int n2 = v2.size();
    int i1 = 0;
    int i2 = 0;

    std::vector<T> res;
    res.reserve(n1+n2);
    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            res.push_back(v1[i1]);
            i1++;
        } else if (v2[i2] < v1[i1]) {
            res.push_back(v2[i2]);
            i2++;
        } else {
            res.push_back(v1[i1]);
            i1++;
            i2++;
        }
    }

    for ( ; i1 < n1; i1++) {
        res.push_back(v1[i1]);
    }

    for ( ; i2 < n2; i2++) {
        res.push_back(v2[i2]);
    }

    return res;
}

/**
 * @brief 计算集合差集
 *
 * @tparam T 集合元素类型
 * @param v1 集合1
 * @param v2 集合2
 *
 * @return 集合1和集合2的差集
 */
template <typename T>
std::vector<T> set_difference(const std::vector<T> &v1, const std::vector<T> &v2)
{
    int n1 = v1.size();
    int n2 = v2.size();
    int i1 = 0;
    int i2 = 0;

    std::vector<T> res;
    res.reserve(n1);

    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            res.push_back(v1[i1]);
            i1++;
        } else if (v2[i2] < v1[i1]) {
            i2++;
        } else {
            i1++;
            i2++;
        }
    }

    for ( ; i1 < n1; i1++) {
        res.push_back(v1[i1]);
    }

    return res;
}

/**
 * @brief 计算集合对称差集
 *
 * @tparam T 集合元素类型
 * @param v1 集合1
 * @param v2 集合2
 *
 * @return 集合1和集合2的对称差集
 */
template <typename T>
std::vector<T> set_symmetric_difference(const std::vector<T> &v1, const std::vector<T> &v2)
{
    int n1 = v1.size();
    int n2 = v2.size();
    int i1 = 0;
    int i2 = 0;

    std::vector<T> res;
    res.reserve(n1+n2);

    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            res.push_back(v1[i1]);
            i1++;
        } else if (v2[i2] < v1[i1]) {
            res.push_back(v2[i2]);
            i2++;
        } else {
            i1++;
            i2++;
        }
    }

    for ( ; i1 < n1; i1++) {
        res.push_back(v1[i1]);
    }

    for ( ; i2 < n2; i2++) {
        res.push_back(v2[i2]);
    }

    return res;
}

}   // namespace mini_utils

#endif	// HEAP_FUNCTIONS
