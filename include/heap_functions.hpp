/**
 * @file heap_functions.hpp
 * @brief 二叉堆相关的函数
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-25
 *
 * @see Data Structures with C++ Using STL, Chapter 14 \n
 * https://github.com/hexu1985/Data.Structures.With.Cpp.Using.STL
 *
 */
#ifndef MINI_UTILS_HEAP_FUNCTIONS_INC
#define MINI_UTILS_HEAP_FUNCTIONS_INC

#include <vector>
#include <functional>

namespace MiniUtils {

/**
 * @brief [0, last-1)范围中的向量元素是一个堆.
 *        将v[last-1]元素插入到堆中, 使[0, last)是一个堆.
 *        使用函数对象comp进行比较.
 *
 * @tparam T 堆中元素类型
 * @tparam Compare 堆中元素比较的函数对象的类型
 * @param v 存放堆元素的数组
 * @param last 插入堆中元素的下标的下一个位置
 * @param comp 元素比较的函数对象
 *
 * @note 如果Compare为less<T>, 那么得到的是一个最小堆.
 *       如果Compare为greater<T>, 那么得到的是一个最大堆.
 */
template <typename T, typename Compare>
void push_heap(std::vector<T> &v, int last, Compare comp);

/**
 * @brief 在索引范围[first, last)中, 下移向量元素v[first]
 *
 * @tparam T 堆中元素类型
 * @tparam Compare 堆中元素比较的函数对象的类型
 * @param v 存放堆元素的数组
 * @param first 需要下移向量元素的索引
 * @param last 索引范围最后一个元素的下一个位置
 * @param comp 元素比较的函数对象
 *
 * @note 如果Compare为less<T>, 那么得到的是一个最小堆.
 *       如果Compare为greater<T>, 那么得到的是一个最大堆.
 */
template <typename T, typename Compare>
void adjust_heap(std::vector<T> &v, int first, int last, Compare comp);

/**
 * @brief [0, last)范围中的向量是堆. 交换堆中的第1个元素和最后一个元素, 然后,
 *        使索引范围[0, last-1)中的元素堆化. 使用函数对象comp进行比较. 
 *
 * @tparam T 堆中元素类型
 * @tparam Compare 堆中元素比较的函数对象的类型
 * @param v 存放堆元素的数组
 * @param last 堆中最后一个元素的下一个位置
 * @param comp 元素比较的函数对象
 *
 * @note 如果Compare为less<T>, 那么得到的是一个最小堆.
 *       如果Compare为greater<T>, 那么得到的是一个最大堆.
 */
template <typename T, typename Compare>
void pop_heap(std::vector<T> &v, int last, Compare comp);

/**
 * @brief 将向量元素安排到堆中. 使用函数对象comp进行比较
 *
 * @tparam T 堆中元素类型
 * @tparam Compare 堆中元素比较的函数对象的类型
 * @param v 存放堆元素的数组
 * @param comp 元素比较的函数对象
 *
 * @note 如果Compare为less<T>, 那么得到的是一个最小堆.
 *       如果Compare为greater<T>, 那么得到的是一个最大堆.
 */
template <typename T, typename Compare>
void make_heap(std::vector<T> &v, Compare comp);

// implementations

template <typename T, typename Compare>
void push_heap(std::vector<T> &v, int last, Compare comp)
{
    // 假设新项的位置为v[last-1], 而且, v[0]到v[last2]的元素以
    // 堆的顺序排列
	int currentPos, parentPos;
	T target;

	// currentPos是用于遍历父节点路径的索引.
    // target是v[last-1]值, 需要在路径中重新定位
	currentPos = last-1;
	parentPos = (currentPos-1)/2;
	target = v[last-1];

	// 遍历父结点的路径, 直到根
	while (currentPos != 0)
	{
		// 比较target和父结点值
		if (comp(target,v[parentPos]))
		{
            // 将数据从父结点位置移动到当前位置.
            // 将表示当前位置的索引更新为父结点位置,
            // 计算下一个父结点位置
			v[currentPos] = v[parentPos];
			currentPos = parentPos;
			parentPos = (currentPos-1)/2;
		}
		else
			// 如果!comp(target, parentvalue), 则满足堆条件
			break;
	}
	// 正确位置已被发现, 将其赋值为target
	v[currentPos] = target;
}

template <typename T, typename Compare>
void adjust_heap(std::vector<T> &v, int first, int last, Compare comp)
{
	int currentPos, childPos;
	T target;

    // target是v[first]值, 需要在路径中重新定位
	currentPos = first;
	target = v[first];

	// 计算左子结点的索引, 开始向下扫描子结点的路径, 在表尾(last)终止,
	// 或者, 在找到target的位置时终止
	childPos = 2 * currentPos + 1;
	while (childPos <= last-1)
	{
        // 右子结点的索引为childPos+1. 比较两个子结点,
        // 如果comp(v[childPos+1], v[childPos])为真, 则改变childPos
		if ((childPos+1 <= last-1) &&
            comp(v[childPos+1], v[childPos]))
			childPos = childPos + 1;

		// 将所选子结点与target进行比较
		if (comp(v[childPos],target))
		{
            // comp(selected child, target)为真, 将所选子结点移到父结点;
            // 现在, 所选子结点的位置空出
			v[currentPos] = v[childPos];

			// 更新索引, 以便继续扫描
			currentPos = childPos;
			childPos = 2 * currentPos + 1;
		}
		else
			// target属于currentPos指定的位置.
			break;
	}
	v[currentPos] = target;
}

template <typename T, typename Compare>
void pop_heap(std::vector<T> &v, int last, Compare comp)
{
	T temp;

	// 交换堆中的第1个元素和最后一个元素
	temp = v[0];
	v[0] = v[last-1];
	v[last-1] = temp;

	// 在[0, last-1)范围中下移根结点
	adjust_heap(v, 0, last-1, comp);
}

template <typename T, typename Compare>
void make_heap(std::vector<T> &v, Compare comp)
{
	int heapPos, lastPos;

    // 从0开始, 并恢复堆的顺序
	lastPos = v.size();
	heapPos = (lastPos - 2)/2;

    // 从最下层的非叶子结点开始堆化, 一直处理到根结点
	while (heapPos >= 0)
	{
		adjust_heap(v, heapPos, lastPos, comp);
		heapPos--;
	}
}

template <typename T, typename Compare>
void sort_heap(std::vector<T> &v, Compare comp)
{
    // 确定元素v[n-1] ... v[1]的迭代
    for (int lastPos = v.size(); lastPos > 1; lastPos--) {
        // 调用pop_heap()将下一个最大结点移到v[n-1]
        pop_heap(v, lastPos, comp);
    }
}

}   // namespace MiniUtils

#endif	// HEAP_FUNCTIONS
