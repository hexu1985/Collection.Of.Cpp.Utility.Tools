/**
 * @file search_functions.hpp
 * @brief 一些简单的查找算法
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-24
 */
#ifndef MINI_UTILS_SEARCH_FUNCTIONS_INC
#define MINI_UTILS_SEARCH_FUNCTIONS_INC

namespace MiniUtils {

/**
 * @brief 顺序查找arr的[first, last)中值等于target的元素的下标
 *
 * @tparam T 元素类型
 * @param arr 被查找的数组
 * @param first 开始索引
 * @param last 结束索引
 * @param target 要查找的目标值
 *
 * @return 返回arr的[first, last)中值等于target的元素的下标, 
 *         如果target不在arr的[first, last)中, 返回last
 */
template <typename T>
int sequence_search(const std::vector<T> &arr, int first, int last, const T &target)
{
    for (int i = first; i < last; i++) {
        if (arr[i] == target)
            return i;
    }
    return last;
}

/**
 * @brief 顺序查找arr中值等于target的元素的下标
 *
 * @tparam T 元素类型
 * @param arr 被查找的数组
 * @param target 要查找的目标值
 *
 * @return 返回arr中值等于target的元素的下标,
 *         如果target不在arr的中, 返回arr.size()
 */
template <typename T>
int sequence_search(const std::vector<T> &arr, const T &target)
{
    return sequence_search(arr, 0, arr.size(), target);
}

/**
 * @brief 二分查找arr的[first, last)中值等于target的元素的下标
 *
 * @tparam T 元素类型
 * @param arr 被查找的数组
 * @param first 开始索引
 * @param last 结束索引
 * @param target 要查找的目标值
 *
 * @return 返回arr的[first, last)中值等于target的元素的下标, 
 *         如果target不在arr的[first, last)中, 返回last
 *
 * @note arr必须是升序排列的, 否则结果未定义
 */
template <typename T>
int binary_search(const std::vector<T> &arr, int first, int last, const T &target)
{
	int mid;					// index of the midpoint
	int origLast = last;		// save original value of last
	
	while (first < last)		// test for nonempty sublist
	{
		mid = (first+last)/2;
		if (target == arr[mid])
			return mid;			// have a match
		// determine which sublist to search
		else if (target < arr[mid])
			last = mid;			// search lower sublist. reset last
		else
			first = mid+1;		// search upper sublist. reset first
	}

	return origLast;			// target not found
}

/**
 * @brief 二分查找arr中值等于target的元素的下标
 *
 * @tparam T 元素类型
 * @param arr 被查找的数组
 * @param target 要查找的目标值
 *
 * @return 返回arr中值等于target的元素的下标,
 *         如果target不在arr的中, 返回arr.size()
 *
 * @note arr必须是升序排列的, 否则结果未定义
 */
template <typename T>
int binary_search(const std::vector<T> &arr, const T &target)
{
    return binary_search(arr, 0, arr.size(), target);
}

/**
 * @brief 二分查找arr的[first, last)中第一个值大于或等于target的元素的下标
 *
 * @tparam T 元素类型
 * @param arr 被查找的数组
 * @param first 开始索引
 * @param last 结束索引
 * @param target 要查找的目标值
 *
 * @return 返回arr的[first, last)中第一个值大于或等于target的元素的下标
 *
 * @note arr必须是升序排列的, 否则结果未定义
 */
template <typename T>
int lower_bound(const std::vector<T> &arr, int first, int last, const T &target)
{
	int mid;					// index of the midpoint
	
	while (first < last)		// test for nonempty sublist
	{
		mid = (first+last)/2;
		// determine which sublist to search
		if (target <= arr[mid])
			last = mid;			// search lower sublist. reset last
		else
			first = mid+1;		// search upper sublist. reset first
	}

	return first;
}

/**
 * @brief 二分查找arr中第一个值大于或等于target的元素的下标
 *
 * @tparam T 元素类型
 * @param arr 被查找的数组
 * @param target 要查找的目标值
 *
 * @return 返回arr中第一个值大于或等于target的元素的下标
 *
 * @note arr必须是升序排列的, 否则结果未定义
 */
template <typename T>
int lower_bound(const std::vector<T> &arr, const T &target)
{
    return lower_bound(arr, 0, arr.size(), target);
}

/**
 * @brief 二分查找arr的[first, last)中第一个值大于target的元素的下标
 *
 * @tparam T 元素类型
 * @param arr 被查找的数组
 * @param first 开始索引
 * @param last 结束索引
 * @param target 要查找的目标值
 *
 * @return 返回arr的[first, last)中第一个值大于target的元素的下标
 *
 * @note arr必须是升序排列的, 否则结果未定义
 */
template <typename T>
int upper_bound(const std::vector<T> &arr, int first, int last, const T &target)
{
	int mid;					// index of the midpoint
	
	while (first < last)		// test for nonempty sublist
	{
		mid = (first+last)/2;
		// determine which sublist to search
		if (target < arr[mid])
			last = mid;			// search lower sublist. reset last
		else
			first = mid+1;		// search upper sublist. reset first
	}

	return first;
}

/**
 * @brief 二分查找arr中第一个值大于target的元素的下标
 *
 * @tparam T 元素类型
 * @param arr 被查找的数组
 * @param target 要查找的目标值
 *
 * @return 返回arr中第一个值大于target的元素的下标
 *
 * @note arr必须是升序排列的, 否则结果未定义
 */
template <typename T>
int upper_bound(const std::vector<T> &arr, const T &target)
{
    return upper_bound(arr, 0, arr.size(), target);
}

}	// namespace MiniUtils

#endif


