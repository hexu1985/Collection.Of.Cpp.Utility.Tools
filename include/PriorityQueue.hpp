/**
 * @file PriorityQueue.hpp
 * @brief 一个优先级队列
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-26
 *
 * @see Data Structures with C++ Using STL, Chapter 14 \n
 * https://github.com/hexu1985/Data.Structures.With.Cpp.Using.STL
 *
 */
#ifndef MINI_UTILS_PRIORITY_QUEUE_INC
#define MINI_UTILS_PRIORITY_QUEUE_INC

#include <vector>
#include <stdexcept>

#include "heap_functions.hpp"

namespace MiniUtils {

/**
 * @brief 维护一个包含数据类型T元素的优先级队列,
 *        使用类型Compare的比较函数对象
 *
 * @tparam T 元素类型
 * @tparam Compare 比较函数对象的类型
 *
 * @note 当Compare为std::greater(大于)的时候, 数值大的元素优先级高;
 *       当Compare为std::less(小于)的时候, 数值小的元素优先级高;
 *       这点与std::priority_queue正好相反.
 */
template <typename T, typename Compare = std::greater<T>> 
class PriorityQueue {                       
public:
    /**
     * @brief 创建空的优先级队列
     */
	PriorityQueue();
 
    /**
     * @brief 返回优先级队列中的元素数目
     *
     * @return 元素个数
     */
	int size() const;

    /**
     * @brief 判断优先级队列是否为空?
     *
     * @return 如果为空, 返回true, 否则返回false
     */
	bool isEmpty() const;

    /**
     * @brief 将item插入到优先级队列中
     *
     * @param item 被插入的元素
     */
	void push(const T &item);

    /**
     * @brief 删除具有最高优先级的元素
     *
     * @warning 如果优先级队列为空, 会抛出std::underflow_error
     */
	void pop();

    /**
     * @brief 返回具有最高优先级的元素
     *
     * @warning 如果优先级队列为空, 会抛出std::underflow_error
     */
	T &top();
	const T &top() const;

private:
	std::vector<T> pqList_; // pqList容纳优先级队列元素
	Compare comp_;          // 比较使用的函数对象
};


template <typename T, typename Compare>
PriorityQueue<T,Compare>::PriorityQueue()
{}


template <typename T, typename Compare>
int PriorityQueue<T,Compare>::size() const
{ 
   return pqList_.size(); 
}

template <typename T, typename Compare>
bool PriorityQueue<T,Compare>::isEmpty() const
{
   return pqList_.empty();
}

template <typename T, typename Compare>
void PriorityQueue<T,Compare>::push(const T &item)
{
	pqList_.push_back(item);
	push_heap(pqList_, pqList_.size(), comp_);
}

template <typename T, typename Compare>
void PriorityQueue<T,Compare>::pop()
{
	if (pqList_.empty())
		throw std::underflow_error("PriorityQueue pop(): empty list");

	pop_heap(pqList_, pqList_.size(), comp_);
	pqList_.pop_back();
}

template <typename T, typename Compare>
T &PriorityQueue<T,Compare>::top()
{
   if (pqList_.empty())
		throw std::underflow_error("PriorityQueue top(): empty list");

	return pqList_[0];
}

template <typename T, typename Compare>
const T &PriorityQueue<T,Compare>::top() const
{
   if (pqList_.empty())
		throw std::underflow_error("PriorityQueue top(): empty list");

	return pqList_[0];
}

}   // namespace MiniUtils

#endif	// PRIORITY_QUEUE
