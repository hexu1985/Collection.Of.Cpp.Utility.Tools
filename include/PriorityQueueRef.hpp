#ifndef MINI_UTILS_REF_PRIORITY_QUEUE_INC
#define MINI_UTILS_REF_PRIORITY_QUEUE_INC

#include <vector>
#include <unordered_map>
#include <functional>
#include <cassert>

namespace MiniUtils {

/**
 * @brief 维护一个包含数据类型T元素的优先级队列,
 *        优先级队列的keyList由外部传入,
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
	Compare comp_;          // 比较使用的函数对象
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
    PriorityQueueRef(const std::vector<T> &keyList): keyList_(keyList) {}

    int isEmpty() const { return pqList_.empty(); }

    void push(int idxOfKeyList)
    { 
        assert(idxOfKeyList >= 0 && idxOfKeyList < keyList_.size());
        pqList_.push_back(idxOfKeyList);
        int i = pqList_.size()-1;
        key2pqMap_[idxOfKeyList] = i;
        bubbleUp(i);
    }

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

    int top()
    {
        assert(!pqList_.empty());
        return pqList_[0];
    }

    void update(int idxOfKeyList)
    {
        assert(key2pqMap_.count(idxOfKeyList) && key2pqMap_[idxOfKeyList] < pqList_.size());
        bubbleUp(key2pqMap_[idxOfKeyList]);
        siftDown(key2pqMap_[idxOfKeyList], pqList_.size());
    }
};

}	// namespace MiniUtils

#endif
