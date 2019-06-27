/**
 * @file heap_sort.hpp
 * @brief 堆排序算法
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-27
 *
 * @see Data Structures with C++ Using STL, Chapter 14 \n
 * https://github.com/hexu1985/Data.Structures.With.Cpp.Using.STL
 *
 */
#ifndef MINI_UTILS_HEAP_SORT_INC
#define MINI_UTILS_HEAP_SORT_INC

#include "heap_functions.hpp"

namespace MiniUtils {

/**
 * @brief 堆排序算法
 *
 * @tparam T 被排序元素类型
 * @tparam Compare 比较函数对象类型
 * @param v 存放的数组元素数组
 * @param comp 比较函数对象
 *
 * @note 如果Compare为less<T>, 那么得到的降序排列,
 *       如果Compare为greater<T>, 那么得到的是升序排列.
 */
template <typename T, typename Compare = std::greater<T>>
void heap_sort(std::vector<T> &v, Compare comp = Compare())
{
    // 堆化向量v
    make_heap(v, comp);

    int i, n = v.size();

    // 确定元素v[n-1] ... v[1]的迭代
    for (int i = n; i > 1; i--) {
        // 调用pop_heap()将下一个最大结点移到v[n-1]
        pop_heap(v, i, comp);
    }
}

}   // namespace MiniUtils

#endif
