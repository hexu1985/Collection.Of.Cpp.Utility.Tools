/**
 * @file sort_functions.hpp
 * @brief 常用排序算法
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-26
 */
#ifndef MINI_UTILS_SORT_FUNCTIONS_INC
#define MINI_UTILS_SORT_FUNCTIONS_INC

#include <vector>
#include "heap_functions.hpp"   // for heap_sort

namespace mini_utils {

template <typename T>
void selection_sort(std::vector<T> &v)
{
    // index of smallest item in each pass
    int smallIndex;
    // save the vector size in n
    int pass, j, n = v.size();
    T temp;

    // sort v[0]..v[n-2], and arr[n-1] is in place
    for (pass = 0; pass < n-1; pass++) {
        // start the scan at index pass; set smallIndex to pass
        smallIndex = pass;
        // j scans the sublist v[pass+1]..v[n-1]
        for (j = pass+1; j < n; j++) {
            // update smallIndex if smaller element is found
            if (v[j] < v[smallIndex]) {
                smallIndex = j;
            }
        }
        // when finished, place smallest item in arr[pass]
        if (smallIndex != pass) {   // swap(v[pass], v[smallIndex])
            temp = v[pass];
            v[pass] = v[smallIndex];
            v[smallIndex] = temp;
        }
    }
}

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
 *
 * @see Data Structures with C++ Using STL, Chapter 14 \n
 * https://github.com/hexu1985/Data.Structures.With.Cpp.Using.STL
 */
template <typename T, typename Compare = std::greater<T>>
void heap_sort(std::vector<T> &v, Compare comp = Compare())
{
    make_heap(v, comp);
    sort_heap(v, comp);
}

}   // namespace mini_utils

#endif
