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
#include "radix_sort.hpp"       // for radix_sort

namespace mini_utils {

template <typename T>
void selection_sort(T arr[], int n)
{
    int smallIndex; // 子表中最小元素的下标
    int pass, j;
    T temp;

    // pass的范围是0 ~ n-2
    for (pass = 0; pass < n-1; pass++)
    {
        // 从下标pass开始扫描子表
        smallIndex = pass;

        // j遍历子表arr[pass+1]到arr[n-1]
        for (j = pass+1; j < n; j++) {
            // 如果找到更小的元素, 则将smallIndex赋值给该位置
            if (arr[j] < arr[smallIndex]) {
                smallIndex = j;
            }
        }

        // 如果smallIndex和pass不在相同的位置,
        // 将子表中的最小项与arr[pass]交换
        if (smallIndex != pass)
        {
            temp = arr[pass];
            arr[pass] = arr[smallIndex];
            arr[smallIndex] = temp;
        }
    }
}

template <typename T>
void selection_sort(std::vector<T> &v)
{
    int smallIndex; // 子表中最小元素的下标
    int pass, j, n = v.size();
    T temp;

    // pass的范围是0 ~ n-2
    for (pass = 0; pass < n-1; pass++) {
        // 从下标pass开始扫描子表
        smallIndex = pass;

        // j遍历子表v[pass+1]到v[n-1]
        for (j = pass+1; j < n; j++) {
            // 如果找到更小的元素, 则将smallIndex赋值给该位置
            if (v[j] < v[smallIndex]) {
                smallIndex = j;
            }
        }

        // 如果smallIndex和pass不在相同的位置,
        // 将子表中的最小项与arr[pass]交换
        if (smallIndex != pass) {
            temp = v[pass];
            v[pass] = v[smallIndex];
            v[smallIndex] = temp;
        }
    }
}

}   // namespace mini_utils

#endif
