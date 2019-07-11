/**
 * @file PriorityQueueRef.hpp
 * @brief 一个基于索引的优先级队列
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-09
 *
 * @see Algorithms in C++, Third Edition, Part 5 Graph Algorithms:
 *      Chapter 20.6, class PQi
 * 
 *
 */
#ifndef MINI_UTILS_PRIORITY_QUEUE_REF_INC
#define MINI_UTILS_PRIORITY_QUEUE_REF_INC

#include <vector>
#include <unordered_map>
#include <functional>
#include <cassert>

namespace MiniUtils {

/**
 * @brief 维护一个包含数据类型T元素的优先级队列,
 *        优先级队列的keyList由外部维护,
 *        优先级队列只维护keyList的索引的堆,
 *        堆化过程只交换索引, 不交换外部实际元素,
 *        使用类型Compare的比较函数对象
 *
 * @tparam T 元素类型
 * @tparam Compare 比较函数对象的类型
 *
 * @note 当Compare为std::greater(大于)的时候, 数值大的元素优先级高;
 *       当Compare为std::less(小于)的时候, 数值小的元素优先级高;
 *       这点与std::priority_queue正好相反.
 */
template <class T, typename Compare = std::greater<T>>
class PriorityQueueRef { 
private:
    std::vector<int> pqList_;                   // 优先级队列, 存放的元素是keyList_中元素的索引
	Compare comp_;                              // 比较使用的函数对象
    std::unordered_map<int, int> key2pqMap_;    // 从keyList_中元素索引, 转换成pqList_中元素索引
    const std::vector<T> &keyList_;             // 存放用于实际计算优先级的key列表(这里是引用, 实际队列有构造函数传入)

    void exch(int i, int j)
    { 
        // 交换pqList_[i]和pqList_[j]
        auto tmp = pqList_[i];
        pqList_[i] = pqList_[j];
        pqList_[j] = tmp;

        // 更新keyList索引到pqList索引的映射
        key2pqMap_[pqList_[i]] = i;
        key2pqMap_[pqList_[j]] = j;
    }

    // 向根结点方向调整堆
    void bubbleUp(int currentPos)
    { 
        int parentPos = (currentPos-1)/2;
        while (currentPos > 0) {
            if (comp_(keyList_[pqList_[currentPos]], keyList_[pqList_[parentPos]])) {
                exch(currentPos, parentPos);
                currentPos = parentPos;
                parentPos = (currentPos-1)/2;
            } else {
				break;
			}
        }
    }

    // 向叶子结点方向调整堆
    void siftDown(int currentPos, int lastPos)
    { 
        int childPos = 2*currentPos+1;
        while (childPos < lastPos) {
            if ((childPos+1 < lastPos) && comp_(keyList_[pqList_[childPos+1]], keyList_[pqList_[childPos]]))
                childPos = childPos + 1;

            if (comp_(keyList_[pqList_[childPos]], keyList_[pqList_[currentPos]])) {
                exch(currentPos, childPos);
                currentPos = childPos;
                childPos = 2*currentPos+1;
            } else {
				break;
			}
        }
    }

public:
    /**
     * @brief 创建空的基于索引的优先级队列
     *
     * @param keyList 外部维护的key列表的引用
     */
    PriorityQueueRef(const std::vector<T> &keyList): keyList_(keyList) {}

    /**
     * @brief 返回优先级队列中的元素数目
     *
     * @return 元素个数
     */
    int size() const { return pqList_.size(); }

    /**
     * @brief 判断优先级队列是否为空?
     *
     * @return 如果为空, 返回true, 否则返回false
     */
    int isEmpty() const { return pqList_.empty(); }

    /**
     * @brief 将keyList[idxOfKeyList]插入到优先级队列中
     *
     * @param idxOfKeyList 被插入元素在keyList中的下标
     */
    void push(int idxOfKeyList)
    { 
        assert(idxOfKeyList >= 0 && idxOfKeyList < keyList_.size());
        pqList_.push_back(idxOfKeyList);
        int i = pqList_.size()-1;
        key2pqMap_[idxOfKeyList] = i;
        bubbleUp(i);
    }

    /**
     * @brief 删除具有最高优先级的元素
     *
     * @return 返回优先级最高元素在keyList中的下标
     */
    int pop() 
    { 
        assert(!pqList_.empty());

        // 交换pqList的0和size-1下标元素,
        // 从pqList的0下标开始heapify pqList
        int idxOfKeyList = pqList_[0];
        int lastPos = pqList_.size()-1;
        exch(0, lastPos);
        siftDown(0, lastPos);

        // pop last element of pqList 
        pqList_.pop_back();
        key2pqMap_.erase(idxOfKeyList);
        return idxOfKeyList;
    }

    /**
     * @brief 返回具有最高优先级的元素
     *
     * @return 返回具有最高优先级的元素在keyList中的下标
     */
    int top()
    {
        assert(!pqList_.empty());
        return pqList_[0];
    }

    /**
     * @brief 更新keyList[idxOfKeyList]的优先级
     *
     * @param idxOfKeyList 被更新优先级的元素在keyList中的下标
     */
    void update(int idxOfKeyList)
    {
        assert(key2pqMap_.count(idxOfKeyList) && key2pqMap_[idxOfKeyList] < pqList_.size());
        bubbleUp(key2pqMap_[idxOfKeyList]);
        siftDown(key2pqMap_[idxOfKeyList], pqList_.size());
    }
};

}	// namespace MiniUtils

#endif
